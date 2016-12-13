#include <vector>
#include <map>
#include <bitset>
#include <stack>

typedef unsigned char byte;

// goals:
// output object data as compact bytes
// take compact bytes and make object + data

class PropertyFormat 
{
public:
	typedef std::pair<byte, byte> element;
	typedef std::vector<element> format_type;
	typedef std::vector<PropertyFormat*> family_type;  // length one array could indicate that that line is to a list of child data branches. this would be bad though as lists are ambiguous (each element of the list shares possible child data formats) and hence need to be processed along with components anyway.
	typedef format_type::size_type index;
    typedef std::map<index, family_type> family_lines_type;
	
	PropertyFormat const &parent;
	index parent_index;
	
	format_type format;
	family_lines_type family_lines;
	
	PropertyFormat* child (index line_index, byte child_id)
	{
		auto family_it = family_lines.find(line_index);
		return (family_it == family_lines.end()) ? nullptr :family_it->second[child_id];
	}
};
template<class el_t, class branch_t = PropertyFormat*>
using leaf_struct = std::map<branch_t, std::vector<el_t>>;
template<class el_t, class variant_t, class branch_t = PropertyFormat*>
using variant_leaf_struct = leaf_struct<el_t, std::pair<branch_t, variant_t>>;

struct Property
{
	typedef PropertyFormat::index index;
	typedef leaf_struct<byte> data_type;  // usage: byte datum = data[&branch_format][datum_index];
	data_type data;
	
	void erase_branch(PropertyFormat* del)
	{
		if (!data.erase(del))
			return;
		std::vector<PropertyFormat*> to_recurse {del};
		while (!to_recurse.empty())						// while there is still something on which to recurse
        {
            PropertyFormat *this_layer = to_recurse.back();
            to_recurse.pop_back();                      // remove this layer from list
			for (auto& p: this_layer->family_lines)	    // then for each possible group of children on this layer
				for (PropertyFormat* next: p.second)   // in fact for each child
					if (data.erase(next))			    // search for and delete the branch associated with that child
						to_recurse.push_back(next);		// and recurse... unless the child wasn't found anyway
        }
	}
	void check_branch(PropertyFormat* check)
	{
		if (data.count(check))
			return;
		for (PropertyFormat* const &sibling: check->parent.family_lines.at(check->parent_index))
			erase_branch(sibling);
		data[check].resize(check->format.size());
	}
	template<class in_it_t>
	in_it_t read_in(in_it_t in_it, PropertyFormat* branch)
	{
	    check_branch(branch);
	    std::vector<byte> &branch_data = data[branch];
	    auto out_it = branch_data.begin();
	    for (std::pair<index, std::vector<PropertyFormat*> >& family: branch->family_lines)
	    {
	        while(out_it != branch_data.begin() + family.first + 1)
	            *out_it++ = *in_it++; // gotta love C++
	        in_it = read_in(in_it, family.second[branch_data[family.first]]);
	    }
        while(out_it != branch_data.end())
            *out_it++ = *in_it++;
        return in_it;
	}
	
	/* recommended to use 
	 * 
	 * std::vector<byte> data;
	 * write_out(std::back_inserter(data), root_branch)
	 */
	template<class out_it_t>
	out_it_t write_out(out_it_t out_it, PropertyFormat* branch) const
	{
	    std::vector<byte> &branch_data = data.at(branch);
	    auto in_it = branch_data.begin();
	    for (std::pair<index, std::vector<PropertyFormat*> >& family: branch->family_lines)
	    {
	        out_it = std::copy(in_it, branch_data.begin() + family.first + 1, out_it);
	        out_it = write_out(out_it, family.second[branch_data[family.first]]);
	    }
        return std::copy(in_it, branch_data.end(), out_it);
	}
	
	
	
};

struct datum {
    // throws out_of_bounds error when invalid, otherwise always returns a value
    byte value = 0;
    PropertyFormat *branch = nullptr;
    byte operator()(Property const &base) const
        {return branch ? base.data.at(branch)[value] : value;}
};

struct comparison {
    // catches all expected errors and therefore always returns
    enum FLAGS 
        {EQU, LSS, GTR, LUNDEF, RUNDEF, TOTAL};
        //FLAGS = 5
    std::bitset<TOTAL> flags;
    datum ld, rd; // left and right data
    bool operator()(Property const &base) const
    {
        byte lv, rv;
      try
        {lv = ld(base);}
      catch(std::out_of_range)
        {return flags[LUNDEF];}
      try
        {rv = rd(base);}
      catch(std::out_of_range)
        {return flags[RUNDEF];}
        
        if (lv == rv)
            return flags[EQU];
        if (lv < rv)
            return flags[LSS];
        return flags[GTR];
    }
};

class template_value_error: public std::logic_error
{
  public:
    template_value_error(std::string const &what_arg):
        std::logic_error(what_arg)
        {}
    template_value_error(char const *what_arg):
        std::logic_error(what_arg)
        {}
};

struct datum_template {
    struct conditioned_datum {
        datum out;
        std::vector<comparison> conditions;
        bool test(Property const& base) const
        {
            for (comparison const &condition: conditions)
                if (!condition(base))
                    return false;
            return true;
        }
    };
    std::vector<conditioned_datum> possibilities;
    byte operator()(Property const &base)
    {
        for (conditioned_datum& possibility: possibilities)
        {
          try
          {
            if (possibility.test(base))
                return possibility.out(base);
          }
          catch(std::out_of_range)
            {;}
        }
        throw template_value_error("Ran out of possibilities for static element");
        
    }
};

struct PropertyTemplate {
    struct branch_template {
        PropertyFormat *output_format;
        std::vector<datum_template> elements;
        std::vector<byte> operator()(Property const &base)
        {
            std::vector<byte> ret;
            ret.reserve(output_format->format.size());
            for (datum_template& element: elements)
                ret.push_back(element(base));
            return ret;
        }
    };
    std::vector<branch_template> branches;
    std::vector<branch_template>::size_type root_num;
    struct stack_entry {
        std::vector<branch_template>::iterator current_branch;
        PropertyFormat::family_lines_type::iterator current_line;
        std::vector<datum_template::conditioned_datum>::iterator current_child_branch;
        
        stack_entry() = default;
        stack_entry(stack_entry const &) = default;
        stack_entry(std::vector<branch_template>::size_type current_branch_ind, std::vector<branch_template> &branches, Property &output)
        {
            current_branch = branches.begin() + current_branch_ind;
            output_data(output).clear();
            output_data(output).reserve(current_branch->output_format->format.size());
            current_line = family_lines().begin();
            current_child_branch = current_child_branches().begin();
        }

        std::vector<byte>& output_data(Property &output) const
            {return output.data[current_branch->output_format];}
        PropertyFormat::family_lines_type& family_lines() const
            {return current_branch->output_format->family_lines;}
        std::vector<datum_template::conditioned_datum>& current_child_branches () const
            {return current_branch->elements[current_line->first].possibilities;}  // takes the array of possible children associated with the current_line iterator, unwrapping the datum_template, since operator() is not needed in this context
        std::vector<datum_template>::size_type prev_element() const
        {
            if (current_line == family_lines().begin()) 
                return 0;
            else
            {
                PropertyFormat::family_lines_type::iterator X(current_line);
                return (--X)->first + 1;
            }
        }
        std::vector<datum_template>::size_type next_element() const
        {
            if (current_line == family_lines().end()) 
                return current_branch->output_format->format.size();
            else
                return current_line->first;
        }
        void find_child_branch(Property const &base)
        {
            while 
            (
              current_child_branch != current_child_branches().end()  // while there are child branches possible to find
              && 
              !(
                current_child_branch->test(base)                      // and this one has bad conditions
                //&& current_child_branch->out.test(base)               // or is a bad reference (for now child branches just need to be literal so dw)
              )
            )
                ++current_child_branch;                               // try the next child branch instead
        }
        void next_child_branch(Property const &base)
        {
            ++current_child_branch;
            find_child_branch(base);
        }
    };
    Property operator()(Property const &base)
    {
        std::stack<stack_entry> entries;
        Property ret;
        std::vector<branch_template>::size_type current_root = 0;
        while (!entries.empty() || current_root < root_num)
        {
          try
          {
        	if (entries.empty())
                entries.emplace(current_root++, branches, ret);
            for (auto current_element = entries.top().prev_element(); current_element < entries.top().next_element(); ++current_element)
                entries.top().output_data(ret).push_back(entries.top().current_branch->elements[current_element](base));
            if (entries.top().current_line != entries.top().family_lines().end())
            {
                entries.top().find_child_branch(base);
                if (entries.top().current_child_branch == entries.top().current_child_branches().end())
                    throw template_value_error("Ran out of possibilities for dynamic element");
                entries.emplace(entries.top().current_child_branch->out(base), branches, ret);
            }
            else
            {
                entries.pop();
                if (entries.empty())
                    break;
                byte child_id = 0; 
              {
                PropertyFormat* child_format = branches[entries.top().current_child_branch->out(base)].output_format;
                std::vector<PropertyFormat*> &child_possibilities = entries.top().current_line->second;
                while (child_id < child_possibilities.size() && child_possibilities[child_id] != child_format) // see issue #18
                    ++child_id;
                if (child_id == child_possibilities.size())
                    throw std::invalid_argument("PropertyTemplate specified child branch that doesn't nest here");
              }
                entries.top().output_data(ret).push_back(child_id);
                ++entries.top().current_line;
            }
          }
          catch (template_value_error)
          {
          	do
          	{
                ret.data.erase(entries.top().current_branch->output_format);
                entries.pop();
                if (!entries.empty())
                    entries.top().next_child_branch(base);
          	} while (!entries.empty() && entries.top().current_child_branch == entries.top().current_child_branches().end());
          }
        }
        return ret;
    }
};