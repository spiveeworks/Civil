
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

struct Property
{
	typedef std::vector<byte> data_type;
	typedef PropertyFormat::index index;
	std::map<PropertyFormat*, data_type> data;
	
	class iterator: public std::iterator<std::bidirectional_iterator, byte>
	{
		PropertyFormat *format;
		Property *property;
		index current;
		
		inline data_type& data () const
		    {return property->data[format];}
		bool is_reduced () const
		    {return current < format->format.size();} // returns false even if irreducible.
		void reduce ()
		{
			while (!is_reduced() && format->parent)
			{
				current = format->parent_index + 1;
				format = format->parent;
			}
		}
		inline std::pair<PropertyFormat*, index> reduced ()
		{
			reduce();
			return std::make_pair(format, current);
		}
		inline std::pair<PropertyFormat*, current> reduced () const
		    {return iterator(*this).reduced();} // what a hack
		
		
		inline byte& operator* ()
		{
			reduce();
			return data()[current];
		}
		byte& operator* () const
		{
			auto x = reduced();
			return property->data[x.first][x.second];
		}
		
		void check_branch(PropertyFormat* check)
		{
			if (property->data.count(check))
			    return;
			for (PropertyFormat* sibling: check->parent->family_lines[check->parent_index])
				if (sibling != check)
					property->data.erase(sibling);
			data[child].resize(child->format.size());
		}
		
        iterator& operator++ () // prefix ++
        {
			reduce();
			PropertyFormat* child = format->child(current, **this);
            if (child) /* currently pointing to substructure id */
			{
			    format = child;
				current = 0;
				check_branch(child);
			}
			else
				current++;
            return *this;
        }
		iterator& operator-- () // prefix --
		{
			if (current == 0)
		    {
				current = format->parent_index;
				format = format->parent;
			}
			else 
			{
				current--;
				for (PropertyFormat* child = format->child(current, **this); child && child->format.size(); child = format->child(current, **this))
				{
					current = child->format.size() - 1;
					format = child;
					check_branch(child);
				} // un-reduces iterator 
			}
		}

        inline iterator operator++ (int)  // postfix ++
        {
           iterator ret(*this);
           ++(*this);
           return ret;
        }
        inline iterator operator-- (int)  // postfix --
        {
           iterator ret(*this);
           --(*this);
           return ret;
        }
		
		inline bool operator== (iterator comp) const
    		{return property == comp.property && reduced() == comp.reduced();}
		inline bool operator!= (iterator comp) const
		    {return !operator==(comp);}
		//idk if these are necessary but they have to be exactly the same as the above which is a pain.
		inline bool operator== (iterator comp)
    		{return property == comp.property && reduced() == comp.reduced();}
		inline bool operator!= (iterator comp)
		    {return !operator==(comp);}
		
		iterator(Property *property_r=nullptr, PropertyFormat *root_format=nullptr, index current_c=0)
		    property(property_r), format(root_format_r), current(current_c)
		    {}
		iterator(iterator const &this_c)
		    iterator(this_c.property, this_c.format, this_c.current)
			{}
	};
	iterator begin()
	    {return iterator(this, root, 0);}
	iterator end()
	    {return iterator(this, root, root->format.size());} //this will try to reduce itself on comparison if (root->parent)
    struct branch_struct
	{
		Property* property;
		PropertyFormat* root;
		iterator begin() const
	        {return iterator(property, root, 0);}
	    iterator end() const
	        {return iterator(property, root, root->format.size());}
	};
	branch_struct branch (PropertyFormat* root) // allows you to do things with for (byte& x: prop.branch(component)) {...}
	    {return branch_struct{this, root};} // please tell me I don't need a ctor
};

class PropertyTemplate {
	
};
