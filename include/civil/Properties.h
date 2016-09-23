#include <spw/leaf_sequence.h>

typedef unsigned char byte;

// goals:
// output object data as compact bytes
// take compact bytes and make object + data

class PropertyFormat 
{
	typedef std::pair<byte, byte> element;
	typedef format_type::size_type index;
	//the rest is required for leaf_sequence
	typedef std::vector<element> format_type;
	typedef std::vector<PropertyFormat*> family_type;
	
	PropertyFormat const &parent;
	index const parent_index;
	
	format_type format;
	std::map<index, family_type> family_lines;
	
	PropertyFormat* child (index line_index, byte child_id)
	{
		auto family_it = family_lines.find(line_index);
		return (family_it == family_lines.end()) ? nullptr :family_it->second[child_it];
	}
};

typedef spw::leaf_sequence<byte, PropertyFormat> Property;

struct thinggggg {
    enum {
        PTO_LITERAL;
        PTO_BASE;
        PTO_BRANCH;
    }pto;
    union{
        byte literal;
        struct base_struct {
            byte index;
            PropertyFormat *branch;
        } base;
        struct branch_struct {
            byte el_id;
            PropertyFormat *branch;
        } branch;
    }; // clearly this union is redundant, as all three possibilities can be represented as pair<byte, PF*>
    // where literal could be make_pair(literal, nullptr)
    // and pto could be stored in the list of instructions
    // or deduced from PropertyFormat->family_lines.count(index)
};

typedef spw::leaf_structure<std::pair<PTO, thinggggg>, PropertyFormat> PropertyTemplate;

bool apply_template(std::vector<byte> &out, Property const &base, PropertyTemplate const &op_tree, PropertyFormat *branch)
{
    auto begin = out.size();    // size before meddling
    for (auto& ops: op_tree.data[branch])
    {
        bool success = false; // this will put a whoole lot of bools on the stack? but they will all be false?
        // also three of the function's arguments are redundant, if optimizer doesn't solve this then making my own stack will.
        //  - but range-based for loops ;.;
        for (auto& op: ops)
        {
            switch (op.pto)
            {
              case PTO_BASE:
              {
                auto branch_it = op_tree.data.find(op.base.branch);
                if (branch_it != op_tree.data.end())
                {
                    out.push_back((*branch_it)[op.base.index])
                    success = true;
                }
                break;
              }
              
              case PTO_LITERAL:
                out.push_back(op.literal)
                success = true;
                break;
                
              case PTO_BRANCH:
                out.push_back(op.branch.el_id)
                if (check_template(out, base, op_tree, op.branch.branch))
                    success = true; // interpretted a subbranch successfully, this element thus also worked
                else
                    out.pop_back()  // something went wrong in the subbranch, try the next option by breaking
                break;
            }
            if (success)
                break;
        }   // end individual element attempts
        if (!success)
        {
            out.resize(begin);  // cut off all changes made by this subroutine
            return false;       // so that in recursive cases caller can try something else
        }
    }   // end this branch
    return true;
}

/*template<class out_it_t>
inline out_it_t apply_template(out_it_t out_it, Property const &base, PropertyTemplate const &op_tree, PropertyFormat *branch)
{
   std::vector<byte> out;
   apply_template(out, base, begin, op_tree, branch);
   return copy(out_it_t, out.begin(), out.end());
}
*/
