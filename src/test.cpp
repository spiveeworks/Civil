#include <Civil/Properties.h>

#include <IOStream>
#include <array>

using namespace std;

template<typename seq_t>
PropertyFormat::format_type make_format(seq_t bytes)
{
    PropertyFormat::format_type out;
    out.reserve(bytes.size() / 2);
    auto it = bytes.begin();
    while (it != bytes.end())
        out.emplace_back(*it++, *it++);  // will throw if an odd number of arguments is given
    return out;
}

void test_property_streaming()
{
    cout << "Beginning bytestream test." << endl;
    
    cout << "Making formats" << endl;
    PropertyFormat format(make_format(array<byte, 2>{1,1}));
    new PropertyFormat(make_format(array<byte, 4>{1,1,1,1}), &format, 0);
    
    cout << "Making containers" << endl;
    Property data;
    std::array<byte, 3> in{0, 1, 2};
    std::vector<byte> out;
        
    cout << "copying array into leaf struct" << endl;
    data.read_in(in.begin(), &format);
    
    cout << "copying leaf struct into vector" << endl;
    data.write_out(back_inserter(out), &format);
    
    cout << "Expected " << (int)  in[0] << ", " << (int)  in[1] << ", " << (int)  in[2] << endl;
    cout << "     Got " << (int) out[0] << ", " << (int) out[1] << ", " << (int) out[2] << endl;
}

string datum_result (datum reader, Property data)
{
    try
        {return to_string((int) reader(data));}
    catch(out_of_range)
        {return "t";}
}

string datum_results (datum reader, Property data_a, Property data_b, Property data_c)
{
    return datum_result(reader, data_a) + ", " + 
           datum_result(reader, data_b) + ", " + 
           datum_result(reader, data_c);
}

void test_template_components()
{
    cout << "Beginning template exctraction test." << endl;
    PropertyFormat root(make_format(array<byte, 6>{1,1,1,1,1,1}));
    PropertyFormat child_0a(make_format(array<byte, 2>{1, 1}),       &root, 0);
    PropertyFormat child_0b(make_format(array<byte, 0>{}),           &root, 0);
    PropertyFormat child_0c(make_format(array<byte, 4>{1, 1, 1, 1}), &root, 0);
    Property data_a, data_b, data_c;
    {
        std::array<byte, 4> in_a{0,4,7,2};
        std::array<byte, 3> in_b{1,7,2};
        std::array<byte, 5> in_c{2,5,6,7,2};
        data_a.read_in(in_a.begin(), &root);
        data_b.read_in(in_b.begin(), &root);
        data_c.read_in(in_c.begin(), &root);
    }
    datum   const_datum{5, nullptr},        // expecting 5
            root_datum{1, &root},           // expecting 7
            child_datum_a{0, &child_0a},    // expecting 4 or throw
            child_datum_c{1, &child_0c};    // expecting 6 or throw  
    
    cout <<  "Const datum, expecting 5, 5, 5 got "  <<  datum_results(  const_datum, data_a, data_b, data_c) << endl;
    cout <<   "Root datum, expecting 7, 7, 7 got "  <<  datum_results(   root_datum, data_a, data_b, data_c) << endl;
    cout << "Child 1 datum, expecting 4, t, t got " <<  datum_results(child_datum_a, data_a, data_b, data_c) << endl;
    cout << "Child 3 datum, expecting t, t, 6 got " <<  datum_results(child_datum_c, data_a, data_b, data_c) << endl;
    
    cout << endl;
    
    comparison     child_a_not_5 {"01110", child_datum_a, const_datum};  // note reversed order in bitset ctor
    comparison child_c_less_root {"00010", child_datum_c,  root_datum};
    
    cout << "child 1 != 5; expecting true, true, true got " 
        << child_a_not_5(data_a) << ", " 
        << child_a_not_5(data_b) << ", "
        << child_a_not_5(data_c) << endl;
    cout << "child 3 < root; expecting false, false, true got " 
        << child_c_less_root(data_a) << ", " 
        << child_c_less_root(data_b) << ", "
        << child_c_less_root(data_c) << endl;
    
    cout << endl;
    
    datum_template::conditioned_datum   
            both  (3, {child_a_not_5, child_c_less_root}),
            one   (2, {child_a_not_5}),
            other (1, {child_c_less_root}),
            neither (0);
    
    datum_template comparator({both, one, other, neither});
    
    cout << "conditioned datum test, expecting 2, 2, 3, got " 
        << (int) comparator(data_a) << ", " 
        << (int) comparator(data_b) << ", " 
        << (int) comparator(data_c) << endl;
}

void test_template_routine()
{
    vector<PropertyFormat> output_formats;
    output_formats.reserve();
    output_formats.emplace_back(make_format(vector<byte>(1, 10*2)));
    output_formats.emplace_back(make_format(vector<byte>(1, 5*2)), &output_formats[0], 2);
    output_formats.emplace_back(make_format(vector<byte>(1, 1*2)), &output_formats[0], 2);
    output_formats.emplace_back(make_format(vector<byte>(1, 1*2)), &output_formats[1], 1);
    output_formats.emplace_back(make_format(vector<byte>(1, 1*2)), &output_formats[1], 3);
    output_formats.emplace_back(make_format(vector<byte>(1, 1*2)), &output_formats[0], 7);
    output_formats.emplace_back(make_format(vector<byte>(1, 0*2)), &output_formats[0], 8);
    
    unsigned data_num = 5;
    std::array<Property, data_num> data;
    {
        std::array<vector<byte>, data_num> content =
        {
            {
                0, 0, 0, 
                    0, 0, 
                        0,
                    0, 0, 
                        0,
                    0, 
                0, 0, 0, 0, 0, 
                    0,
                0, 0
            },
            {},
            {},
            {},
            {},
        }
        for (unsigned i = 0; i < data_num; ++i)
            data[i].read_in(content[i].begin(), &output_formats[0]);
    }
    
    
    
}

int main()
{
    test_property_streaming();
    cout << endl << endl;
    test_template_components();
    cout << endl << endl;
    test_template_routine();
}
