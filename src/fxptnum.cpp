template<typename T1, typename T2>
struct biggest_
{
    typedef if_<(sizeof(T1)>=sizeof(T2)),T1,T2> type;
};

/*
 * This class is designed to be a scalable int template,
 * allowing for numbers to be counted by 2s, 4s, 1/2s, etc
 * and exists purely to make inter-scale casting neat and implicit
 * it also allows for the represented numbers to be multiplied
 * rather than the internal mantissa
 */

template<int E, typename IntType = int>
class FxPtNum {
	IntType mantissa = 0;
public:
	template<int altE, typename altIntType>
	FxPtNum<E + altE, biggest_<IntType,altIntType>::type> 
	// in other words, the new exponent is the sum of the arguments, and the new mantissa is as precise as the largest mantissa given
	operator* (FxPtNum<altE, altIntType> const &that)
	{
		FxPtNum<E + altE, biggest_<IntType,altIntType>::type> 
		ret.mantissa = mantissa * that.mantissa; //casting these would improve reliability in weird situations, but would make this class even harder to read
		return ret;
	}
	
	template<int altE, typename altIntType>
	FxPtNum<E - altE, biggest_<IntType,altIntType>::type> 
	// in other words, the new exponent is the sum of the arguments, and the new mantissa is as precise as the largest mantissa given
	operator* (FxPtNum<altE, altIntType> const &that)
	{
		FxPtNum<E - altE, biggest_<IntType,altIntType>::type> ret;
		ret.mantissa = mantissa / that.mantissa; //consider increasing precision before some division operations, as this must be done manually.
		return ret;
	}
		
		
		
	FxPtNum (IntType const &base = 0)
	{
		mantissa = base >> E; 
	}
	FxPtNum (double const &base)
	{
	    mantissa = ldexp(base,-E);
	}
	template<int altE, typename altIntType>
	FxPtNum (FxPtNum<altE, altIntType> const &base)
	{
	    mantissa = base.mantissa >> (E - altE);
	}
	
	operator double() const 
	{
	    return ldexp((double)mantissa, E);
	}
	operator IntType() const
	{
	    return mantissa << E;
	}
};


