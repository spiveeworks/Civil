
/* format_t needs to meet the following
 *
	typedef format_type::size_type index
	typedef std::vector<PropertyFormat*> family_type;
	format_type format;
	std::map<index, family_type> family_lines;
	
	PropertyFormat const &parent;
	index const parent_index;
	
	PropertyFormat* child (index line_index, byte child_id)
	{
		auto family_it = family_lines.find(line_index);
		return (family_it == family_lines.end()) ? nullptr :family_it->second[child_it];
	}
 */
namespace spw
	{
template<typename el_t, typename format_t>
struct leaf_sequence
{
	typedef std::vector<el_t> branch_type;
	typedef format_t::index index;
	typedef std::map<format_t*, branch_type> data_type;
	data_type data;
	
	void erase(format_t* del)
	{
		if (!data.erase(del))
			return;
		std::vector<format_t*> to_recurse = {del};
		while (to_recurse) 												// while there is still something on which to recurse
			for (auto& p: to_recurse.pop_back()->first->family_lines)	// remove one thing to recurse, and, for each possible group of children
				for (format_t* next: p->second)					// in fact for each child
					if (data.erase(next))								// search for and delete the branch associated with that child
						todel.push_back(next);							// and recurse... unless the child wasn't found anyway
	}
	void check_branch(format_t* check)
	{
		if (data.count(check))
			return;
		for (format_t* sibling: check->parent->family_lines[check->parent_index])
			erase_branch(sibling);
		data[child].resize(child->format.size());
	}
	
	struct iterator: public std::iterator<std::bidirectional_iterator, byte>
	{
		format_t *format;
		leaf_sequence *tree;
		index current;
		
		inline data_type& data () const
		    {return tree->data[format];}
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
		inline std::pair<format_t*, index> reduced ()
		{
			reduce();
			return std::make_pair(format, current);
		}
		inline std::pair<format_t*, current> reduced () const
		    {return iterator(*this).reduced();} // what a hack
		
		
		inline el_t& operator* ()
		{
			reduce();
			return data()[current];
		}
		el_t& operator* () const
		{
			auto x = reduced();
			return tree->data[x.first][x.second];
		}
		el_t* operator-> ()
		    {return &**this;}
		el_t* operator-> () const
		    {return &**this;}
		
		
        iterator& operator++ () // prefix ++
        {
			reduce();
			format_t* child = format->child(current, **this);
            if (child) /* currently pointing to substructure id */
			{
			    format = child;
				current = 0;
				tree->check_branch(child);
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
				for (format_t* child = format->child(current, **this); child && child->format.size(); child = format->child(current, **this))
				{
					current = child->format.size() - 1;
					format = child;
					tree->check_branch(child);
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
    		{return tree == comp.tree && reduced() == comp.reduced();}
		inline bool operator!= (iterator comp) const
		    {return !operator==(comp);}
		//idk if these are necessary but they have to be exactly the same as the above which is a pain.
		inline bool operator== (iterator comp)
    		{return tree == comp.tree && reduced() == comp.reduced();}
		inline bool operator!= (iterator comp)
		    {return !operator==(comp);}
		
		iterator(tree *tree_r=nullptr, format_t *root_format=nullptr, index current_c=0)
		    tree(tree_r), format(root_format_r), current(current_c)
		    {}
		iterator(iterator const &this_c)
		    iterator(this_c.tree, this_c.format, this_c.current)
			{}
	};
	iterator begin()
	    {return iterator(this, root, 0);}
	iterator end()
	    {return iterator(this, root, root->format.size());} //this will try to reduce itself on comparison if (root->parent)
    struct branch_struct
	{
		leaf_sequence* tree;
		format_t* root;
		iterator begin() const
	        {return iterator(tree, root, 0);}
	    iterator end() const
	        {return iterator(tree, root, root->format.size());}
	};
	branch_struct branch (format_t* root) // allows you to do things with for (byte& x: prop.branch(component)) {...}
	    {return branch_struct{this, root};} // please tell me I don't need a ctor
};

	}
