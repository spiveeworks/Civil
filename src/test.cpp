//taxation is theft
#include <Civil/tax.h>

#include <IOStream>
#include <array>
#include <memory>

using namespace std;
using namespace trg;

template<typename seq_t>
Format::format_type make_format(seq_t bytes)
{
    Format::format_type out;
    out.reserve(bytes.size() / 2);
    auto it = bytes.begin();
    while (it != bytes.end())
        out.emplace_back(*it++, *it++);  // will throw if an odd number of arguments is given
    return out;
}

void test_taxstr_streaming()
{
    cout << "Beginning bytestream test." << endl;
    
    cout << "Making formats" << endl;
    Format format(make_format(array<byte, 2>{1,1}));
    new Format(make_format(array<byte, 4>{1,1,1,1}), &format, 0);
    
    cout << "Making containers" << endl;
    String data;
    std::array<byte, 3> in{0, 1, 2};
    std::vector<byte> out;
        
    cout << "copying array into leaf struct" << endl;
    data.read_in(in.begin(), &format);
    
    cout << "copying leaf struct into vector" << endl;
    data.write_out(back_inserter(out), &format);
    
    cout << "Expected " << (int)  in[0] << ", " << (int)  in[1] << ", " << (int)  in[2] << endl;
    cout << "     Got " << (int) out[0] << ", " << (int) out[1] << ", " << (int) out[2] << endl;
}

string datum_result (datum reader, String data)
{
    try
        {return to_string((int) reader(data));}
    catch(out_of_range)
        {return "t";}
}

string datum_results (datum reader, String data_a, String data_b, String data_c)
{
    return datum_result(reader, data_a) + ", " + 
           datum_result(reader, data_b) + ", " + 
           datum_result(reader, data_c);
}

void test_template_components()
{
    cout << "Beginning template exctraction test." << endl;
    Format root(make_format(array<byte, 6>{1,1,1,1,1,1}));
    Format child_0a(make_format(array<byte, 2>{1, 1}),       &root, 0);
    Format child_0b(make_format(array<byte, 0>{}),           &root, 0);
    Format child_0c(make_format(array<byte, 4>{1, 1, 1, 1}), &root, 0);
    String data_a, data_b, data_c;
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
    
    comparison     child_a_not_5 {"001110", child_datum_a, const_datum};  // note reversed order in bitset ctor
    comparison child_c_less_root {"000010", child_datum_c,  root_datum};
    
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

string read_string(String data, Format *format)
{
    vector<byte> read;
    data.write_out(back_inserter(read), format);
    string out;
    for (byte x: read)
        out += to_string((int)x) + ", ";
    return out.substr(0, out.size() - 2);
}

void test_template_routine()
{
    cout << "Making output format" << endl;
    vector<unique_ptr<Format> > output_formats;
    output_formats.reserve(7);
    cout << "Making first one" << endl;
    output_formats.emplace_back(new Format(make_format(vector<byte>(10*2, 1))));
    cout << "Making second one" << endl;
    output_formats.emplace_back(new Format(make_format(vector<byte>(5*2, 1)), output_formats[0].get(), 2));
    output_formats.emplace_back(new Format(make_format(vector<byte>(3*2, 1)), output_formats[0].get(), 2));
    output_formats.emplace_back(new Format(make_format(vector<byte>(1*2, 1)), output_formats[1].get(), 1));
    output_formats.emplace_back(new Format(make_format(vector<byte>(1*2, 1)), output_formats[1].get(), 3));
    output_formats.emplace_back(new Format(make_format(vector<byte>(1*2, 1)), output_formats[0].get(), 7));
    output_formats.emplace_back(new Format(make_format(vector<byte>(0*2, 1)), output_formats[0].get(), 8));
    
    cout << "Making input format";
    Format input_root(make_format(vector<byte>(1*2, 1)));
    Format input_child_null(vector<pair<byte, byte> >(), &input_root, 0);
    Format input_child(make_format(vector<byte>(4*2, 1)), &input_root, 0);
    
    cout << "Making input data";
    constexpr unsigned data_num = 2;
    std::array<String, data_num> data;
    {
        std::array<vector<byte>, data_num> content =
        {
            /*{
                0, 0, 0, 
                    0, 0, 
                        0,
                    0, 0, 
                        0,
                    0, 
                0, 0, 0, 0, 0, 
                    0, 0, 0,
                0, 0
            },*/
            vector<byte>{0},
            vector<byte>{1, 0, 0, 0, 0},
        };
        for (unsigned i = 0; i < data_num; ++i)
            data[i].read_in(content[i].begin(), &input_root);
    }
    cout << "Making template parts" << endl;
    datum zero_datum(0);
    datum_template zero_datum_template{{zero_datum}};
    cout << "Making template" << endl;
    Template const_gen{{
        {output_formats[2].get(), vector<datum_template>(3, zero_datum_template)}
    }, 1};
    cout << "Successfully prepared the stuff" << endl;
    cout << "Expecting 0, 0, 0, got " << read_string(const_gen(data[0]), output_formats[2].get()) << endl;
}

int main()
{
    test_taxstr_streaming();
    cout << endl << endl;
    test_template_components();
    cout << endl << endl;
    test_template_routine();
}
