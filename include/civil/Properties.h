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
        
    }pto;
    union{
        byte statit_el;
        
    };
};

typedef spw::leaf_sequence<std::pair<PTO, thinggggg>, PropertyFormat> PropertyTemplate;

template<class out_it_t>
out_it_t apply_template(out_it_t out_it, Property const &base, PropertyTemplate::iterator template_begin, PropertyTemplate::iterator template_end)
{
   for (auto& ops = *template_begin; template_begin != template_end; ops = *++template_begin)
   {
       bool success = false;
       for (auto& op: ops)
       {
           switch(op.pto)
           {
              case PTO_STATIC:
                *out_it++ = op.static_el;
                success = true;
                break;
              case PTO_BASE:
                
                
           }
           if (success)
               break;
       }
       if (!success)
           ; // PROBLEMS
   }
   return out_it;
   }
}
}

