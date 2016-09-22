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


class PropertyTemplate spw::leaf_sequence<std::pair<PTO, thinggggg>, PropertyFormat>;
class PropertyTemplate::iterator
{
	Property::iterator base;
	PropertyTemplateObject::iterator op;
	iterator& operator++()
	{
		switch(op->first)
		{
		  case PTO_STATIC:
			++op;
			break;
		  case PTO_STATIC_BRANCH:
			op.rebranch(op->second);
			//op = PropertyTemplateObject::iterator(op.tree, op.format->child(op.current, op->second));
			break;
		  case PTO_BASE:
			++op;
			++base;
			break;
		  case PTO_BASE_BRANCH:
			break;
		}
		while(
		  case PTO_TEMPLATE_BRANCH:
			op.rebranch(*base);
			++base;
			break;
	}
};


