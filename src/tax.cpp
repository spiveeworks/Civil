#include <Civil/tax.h>

#include <stack>

using namespace trg;

// ********** //
// * Format * //
// ********** //

Format::family_type::size_type Format::child_id(family_lines_type::const_iterator line, Format *child_format) const
// see issue #18
{
    std::vector<Format*> const &child_possibilities = line->second;
    for 
    (
        Format::family_type::size_type child_id = 0;
        child_id < child_possibilities.size(); 
        ++child_id
    )
        if (child_possibilities[child_id] == child_format)
            return child_id;
    throw std::invalid_argument("specified child branch that doesn't nest here");
}

// ********** //
// * String * //
// ********** //

void String::erase_branch(Format* del)
{
    if (!data.erase(del))
        return;
    std::vector<Format*> to_recurse {del};
    while (!to_recurse.empty())						// while there is still something on which to recurse
    {
        Format *this_layer = to_recurse.back();
        to_recurse.pop_back();                      // remove this layer from list
        for (auto& p: this_layer->family_lines)	    // then for each possible group of children on this layer
            for (Format* next: p.second)    // in fact for each child
                if (data.erase(next))			    // search for and delete the branch associated with that child
                    to_recurse.push_back(next);		// and recurse... unless the child wasn't found anyway
    }
}

void String::check_branch(Format* check)
{
    if (data.count(check))
        return;
    if (check->parent)
        for (Format* const &sibling: check->parent->family_lines.at(check->parent_index))
            erase_branch(sibling);
    data[check].resize(check->format.size());
}

// ************ //
// * Template * //
// ************ //

Template::stack_entry::stack_entry(std::vector<branch_template>::size_type current_branch_ind, std::vector<branch_template> &branches, String &output)
{
    current_branch = branches.begin() + current_branch_ind;
    output_data(output).clear();
    output_data(output).reserve(current_branch->output_format->format.size());
    current_line = family_lines().begin();
    begin_child_branches();
}

String Template::operator()(String const &base)
{
    std::stack<stack_entry> entries;
    String ret;
    std::vector<branch_template>::size_type current_root = 0;
    while (!entries.empty() || current_root < root_num)
    {
      try
      {
        if (entries.empty())
            entries.emplace(current_root++, branches, ret);
        ProcessStatics(entries.top(), ret, base);
        if (entries.top().current_line != entries.top().family_lines().end())
        {
            //find possible child branch
            entries.top().begin_child_branches();
            entries.top().find_child_branch(base);
            if (entries.top().current_child_branch == entries.top().current_child_branches().end())
                throw template_value_error("Ran out of possibilities for dynamic element");
            //push to stack to process next iteration
            entries.emplace(entries.top().current_child_branch->out(base), branches, ret);
        }
        else
        {
            //get format for parent branch to knoww
            Format *child_format = entries.top().current_branch->output_format;
            //pop finished branch
            entries.pop();
            //empty stack means this is final iteration
            if (entries.empty())
                break;
            //find child id using Format*
            auto child_id = entries.top().current_branch->output_format->child_id(entries.top().current_line, child_format);
            //append it to parent branch
            entries.top().output_data(ret).push_back(child_id);
            //get ready for next iteration
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
