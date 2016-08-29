
typedef unsigned char byte;

// goals:
// output object data as compact bytes
// take compact bytes and make object + data

class EntityClass 
{
	
};

typedef byte penum;

struct Property 
{
	typedef std::vector<byte> data_type;
	std::map<byte,data_type> data;
	
	class iterator
	{
		PropertyTemplate const &format;
		Property& property;
		byte context, index;
		iterator* parent;
		byte& operator* () const
			{return property.data[context][index];}
		
        iterator operator++ () // prefix ++
        {
			iterator& this_after = (parent && index + 1 < property.data[context].size()) ?
				*this:
				*parent;
			
            if ( /* currently pointing to substructure id */ )
			{
			    iterator ret = new iterator(this_after, *this_after);
				index++;
				return ret;
			}
			index++;
            return this_after;
        }

        iterator operator++ (int)  // postfix ++
        {
           iterator ret(*this);
           ++(*this);
           return ret;
        }
		
		iterator();//////////
	}
};

class PropertyTree {
	struct entry_format {
		byte id;
		byte index;
	};
	typedef std::vector<entry_format> context;
	typedef penum penum_size;
	constexpr byte ID_SUBPROPERTY 0;
	constexpr byte ID_PENUM 1;
	std::vector<context> sub_property;
	std::vector<penum_size> penums;
	
	//Maybe these should use iterators? but templatesss
	void read (Property& output, byte* input) const
	{
		
	}
	void write (byte* output, Property const &input) const
	{
		
	}
}*property_tree;

class PropertyTemplate {
	
};