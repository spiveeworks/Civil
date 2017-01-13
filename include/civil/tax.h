#include <vector>
#include <map>
#include <bitset>

typedef unsigned char byte;

    namespace trg { // taxonomic regular grammar

class Format 
{
public:
	typedef std::pair<byte, byte> element;
	typedef std::vector<element> format_type;
	typedef std::vector<Format*> family_type;  // length one array could indicate that that line is to a list of child data branches. this would be bad though as lists are ambiguous (each element of the list shares possible child data formats) and hence need to be processed along with components anyway.
	typedef format_type::size_type index;
    typedef std::map<index, family_type> family_lines_type;
	
	Format *parent;  // const correctness?
	index parent_index;
	
	format_type format;
	family_lines_type family_lines;
	
	Format* child (index line_index, byte child_id) const
    {
        auto family_it = family_lines.find(line_index);
        return (family_it == family_lines.end()) ? nullptr :family_it->second[child_id];
    }
    
    // this overload exists for Template, which has a stack of iterators, not indeces
    family_type::size_type child_id(family_lines_type::const_iterator line, Format *child_format) const;

    family_type::size_type child_id(index line_index, Format *child_format) const
    {
        auto line = family_lines.find(line_index);
        // think of it as an empty vector, find will always fail
        if (line == family_lines.end())
            throw std::invalid_argument("specified child branch that doesn't nest here - line is empty");
        return child_id(line, child_format);
    }
    
    Format (format_type format_c, Format *parent_c=nullptr, index parent_index_c=0):
        format(format_c),
        parent(parent_c),
        parent_index(parent_index_c)
        {if (parent) parent->family_lines[parent_index].push_back(this);}
    
    void check_children()
    {
        for (auto const &line: family_lines)
            for (Format *child: line.second)
                child->parent = this;
    }
    
    void clear_family()
    {
        parent = nullptr;
        family_lines.clear();
    }
    
    Format(Format &&base):
        format(base.format),
        family_lines(base.family_lines),
        parent(base.parent),
        parent_index(base.parent_index)
        {
            check_children();
            std::vector<Format*> &siblings = parent->family_lines[parent_index];
            for(auto it = siblings.begin(); it != siblings.end(); ++it) // see issue #18
                if (*it == &base)
                    *it = this;
            base.clear_family(); //?
        }
};

struct String
// is a string within the grammar defined by a trg::Format
{
	typedef Format::index index;
	typedef std::map<Format*, std::vector<byte> > data_type;  // usage: byte datum = data[&branch_format][datum_index];
	data_type data;
	
	void erase_branch(Format* del);
	void check_branch(Format* check);
    
    // takes a byte stream through an input iterator, and creates a structure of the given format
	template<class in_it_t>
	in_it_t read_in(in_it_t in_it, Format* branch);
	
	/* recommended to use 
	 * 
	 * std::vector<byte> data;
	 * write_out(std::back_inserter(data), root_branch)
	 */

    // outputs a byte stream from the data of a given format
	template<class out_it_t>
	out_it_t write_out(out_it_t out_it, Format* branch) const;
};

template<class in_it_t>
in_it_t String::read_in(in_it_t in_it, Format* branch)
{
    check_branch(branch);
    std::vector<byte> &branch_data = data[branch];
    auto out_it = branch_data.begin();
    for (std::pair<index, std::vector<Format*> > const &family: branch->family_lines)
    {
        while(out_it != branch_data.begin() + family.first + 1)
            *out_it++ = *in_it++; // gotta love C++
        in_it = read_in(in_it, family.second[branch_data[family.first]]);
    }
    while(out_it != branch_data.end())
        *out_it++ = *in_it++;
    return in_it;
}

template<class out_it_t>
out_it_t String::write_out(out_it_t out_it, Format* branch) const
{
    std::vector<byte> const &branch_data = data.at(branch);
    auto in_it = branch_data.begin();
    for (std::pair<index, std::vector<Format*> > const &family: branch->family_lines)
    {
        out_it = std::copy(in_it, branch_data.begin() + family.first + 1, out_it);
        in_it = branch_data.begin() + family.first + 1;
        out_it = write_out(out_it, family.second[branch_data[family.first]]);
    }
    return std::copy(in_it, branch_data.end(), out_it);
}

struct datum {
    // throws out_of_bounds error when invalid, otherwise always returns a value
    byte value;
    Format *branch;
    byte operator()(String const &base) const
        {return branch ? base.data.at(branch)[value] : value;}
    
    datum (byte value_c = 0, Format *branch_c = nullptr):
        value(value_c),
        branch(branch_c)
        {}
    datum (datum const &) = default;
    datum& operator= (datum const &) = default;
};

struct comparison {
    // catches all expected errors and therefore always returns
    enum FLAGS 
        {EQU, LSS, GTR, LUNDEF, RUNDEF, LRUNDEF, TOTAL};
        //FLAGS = 6
    std::bitset<TOTAL> flags;
    datum ld, rd; // left and right data
    bool operator()(String const &base) const
    {
        byte lv, rv;
      {
          bool lundef = false;
        try
          {lv = ld(base);}
        catch(std::out_of_range)
          {lundef = true;}
        try
          {rv = rd(base);}
        catch(std::out_of_range)
          {return lundef ? flags[LRUNDEF] : flags[RUNDEF];}

          if (lundef)
              return flags[LUNDEF];
      }
        if (lv == rv)
            return flags[EQU];
        if (lv < rv)
            return flags[LSS];
        return flags[GTR];
    }
    template<typename bitit>
    comparison(bitit flags_c, datum ld_c, datum rd_c):
        flags(flags_c),
        ld(ld_c),
        rd(rd_c)
        {}
    comparison() = default;
    comparison(comparison const&) = default;
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
        bool test(String const& base) const
        {
            for (comparison const &condition: conditions)
                if (!condition(base))
                    return false;
            return true;
        }
        
        conditioned_datum(datum out_c, std::vector<comparison> conditions_c = {}):
            out(out_c),
            conditions(conditions_c)
            {}
        
        conditioned_datum(conditioned_datum const &) = default;
        conditioned_datum& operator= (conditioned_datum const &) = default;
        conditioned_datum(conditioned_datum&&) = default;
    };
    std::vector<conditioned_datum> possibilities;
    byte operator()(String const &base)
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
    
    datum_template(std::vector<conditioned_datum> possibilities_c):
        possibilities(possibilities_c)
        {}
};

class Template {
  public:
    struct branch_template {
        Format *output_format; // could be more efficient to store parent index, and deduce the format by parent's format
        std::vector<datum_template> elements;
        
        branch_template(branch_template const&) = default;
        branch_template() = default;
        branch_template(Format *output_format_c, std::vector<datum_template> elements_c):
            output_format(output_format_c),
            elements(elements_c)
            {}
    };
  private:
    struct stack_entry {
        std::vector<branch_template>::iterator current_branch;
        Format::family_lines_type::iterator current_line;
        std::vector<datum_template::conditioned_datum>::iterator current_child_branch;
        
        stack_entry() = default;
        stack_entry(stack_entry const &) = default;
        stack_entry(std::vector<branch_template>::size_type current_branch_ind, std::vector<branch_template> &branches, String &output);

        std::vector<byte>& output_data(String &output) const
            {return output.data[current_branch->output_format];}
        Format::family_lines_type& family_lines() const
            {return current_branch->output_format->family_lines;}
        std::vector<datum_template::conditioned_datum>& current_child_branches () const
            {return current_branch->elements[current_line->first].possibilities;}  // takes the array of possible children associated with the current_line iterator, unwrapping the datum_template, since operator() is not needed in this context
        void begin_child_branches ()
            {current_child_branch = current_child_branches().begin();}
        std::vector<datum_template>::size_type prev_element() const
        {
            if (current_line == family_lines().begin()) 
                return 0;
            else
            {
                Format::family_lines_type::iterator X(current_line);
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
        void find_child_branch(String const &base)
        {
            while (current_child_branch != current_child_branches().end() && !current_child_branch->test(base))
                ++current_child_branch;
        }
        void next_child_branch(String const &base)
        {
            ++current_child_branch;
            find_child_branch(base);
        }
        void next_family_line()
        {
            ++current_line;
            if (current_line != family_lines().end())
                begin_child_branches();
        }
    };

    std::vector<branch_template> branches;
    std::vector<branch_template>::size_type root_num;

    void ProcessStatics(stack_entry const &top, String &ret, String const &base)
    {
        for (auto current_element = top.prev_element(); current_element < top.next_element(); ++current_element)
            top.output_data(ret).push_back(top.current_branch->elements[current_element](base));
    }

  public:
    String operator()(String const &base);
    
    Template (std::vector<branch_template> branches_c, std::vector<branch_template>::size_type root_num_c):
        branches(branches_c),
        root_num(root_num_c)
        {}
};

    } // end namespace trg
