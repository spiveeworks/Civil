
typedef unsigned char byte;

// goals:
// output object data as compact bytes
// take compact bytes and make object + data

class EntityClass 
{
	
};

typedef byte penum;

class PropertyFormat 
{
	typedef std::pair<byte, byte> element;
	typedef std::vector<element> format_type;
	typedef std::vector<PropertyFormat*> family_type;
	typedef format_type::size_type index
	
	PropertyFormat const &parent;
	index parent_index;
	
	format_type format;
	std::map<index, family_type> family_lines;
	
	PropertyFormat* child (index line_index, byte child_id)
	{
		auto family_it = family_lines.find(line_index);
		return (family_it == family_lines.end()) ? nullptr :family_it->second[child_it];
	}
};
template<class el_t>
using leaf_struct = std::map<PropertyFormat*, std::vector<el_t> >;
struct Property
{
	typedef PropertyFormat::index index;
	typedef leaf_struct<byte> data_type;  // usage: byte datum = data[&branch_format][datum_index];
	data_type data;
	
	void erase(PropertyFormat* del)
	{
		if (!data.erase(del))
			return;
		std::vector<data_type::iterator> to_recurse = {del};
		while (to_recurse) 												// while there is still something on which to recurse
			for (auto& p: to_recurse.pop_back()->first->family_lines)	// remove one thing to recurse, and, for each possible group of children
				for (PropertyFormat* next: p->second)					// in fact for each child
					if (data.erase(next))								// search for and delete the branch associated with that child
						todel.push_back(next);							// and recurse... unless the child wasn't found anyway
	}
	void check_branch(PropertyFormat* check)
	{
		if (property->data.count(check))
			return;
		for (PropertyFormat* sibling: check->parent->family_lines[check->parent_index])
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
	            *out_it++ = *in_it++; // I love C++
	        in_it = read_in(in_it, family.second[branch_data[family.first]])
	    }
        while(out_it != branch_data.end())
            *out_it++ = *in_it++;
        return in_it;
	}
	
	template<class out_it_t>
	out_it_t write_out(out_it_t out_it, PropertyFormat* branch) const
	{
	    std::vector<byte> &branch_data = data.at(branch);
	    auto in_it = branch_data.begin();
	    for (std::pair<index, std::vector<PropertyFormat*> >& family: branch->family_lines)
	    {
	        out_it = std::copy(in_it, branch_data.begin() + family.first + 1, out_it)
	        out_it = write_out(out_it, family.second[branch_data[family.first]])
	    }
        return std::copy(in_it, branch_data.end(), out_it);
	}
	
	
	
};

typedef std::pair<PropertyFormat*, byte> leaf_index;
typedef leaf_struct<std::vector<leaf_index> > PropertyTemplate; // vector? why not forward_list?

bool apply_leaf_indeces(std::vector<byte>& out, Property const &base, std::vector<std::vector<leaf_index> > const &op_branch, index copy_from, index copy_to)
{// my attempt at making the code less monstrous by making two functions didn't do so well
    index curr_size = out.size();
    out.resize(curr_size + copy_to - copy_from);
    auto out_it = out.end() - copy_to;  // such that out_it[copy_from] will currently refer to the first of the new bytes
    while(copy_from < copy_to)
    {
        bool success = false;
        for (leaf_index p: op_branch[copy_from])
        {
            if (!p.first)
            {
                out_it[copy_from] = p.second;
                success = true;
                break;
            }
            auto branch_it = base.data.find(p.first);
            if branch_it == base.data.end()
                continue;
            out = (*branch_it)[p.second];
            return true;
        }
        if(!success)
            return false;
        copied++;
    }
}

bool apply_template(std::vector<byte>& out, Property const &base, PropertyTemplate const &op_tree, PropertyFormat* branch)
{// lol good luck debugging
    using index = std::vector<byte>::size_type;
    index prev_size = out.size();
    auto const &op_branch = op_tree.at(branch);
    index copied = 0;
    for (std::pair<index, std::vector<PropertyFormat*> >& family: branch->family_lines)
    {
        if (!apply_leaf_indeces(out, base, op_branch, copied, family.first))
        {
            out.resize(prev_size);
            return false;
        }
        copied = family.first();
        bool child_success = false;
        for(leaf_index p: op_branch[copied])
        {
            //special meaning if (p.first)?
            out.push_back(p.second);
            child_success = apply_template(out, base, op_tree, family.second[copied]);
            if (child_success)
                break;
            out.pop_back();
        }
        if(!child_success)
        {
            out.resize(prev_size);
            return false;
        }
    }
    if (!apply_leaf_indeces(out, base, op_branch, copied, op_branch.size()))
    {
        out.resize(prev_size);
        return false;
    }
    return true;
}

