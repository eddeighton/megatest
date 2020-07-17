

#include <memory>

#include "egcomponent/traits.hpp"

namespace eg
{
	
template<>
struct DimensionTraits< std::shared_ptr< int > >
{
    using T = std::shared_ptr< int >;
    using Read  = T;
    using Write = T;
    using Get   = T&;
    static const std::size_t Size = sizeof( T );
    static const std::size_t Simple = false;//std::is_trivially_copyable< T >::value;
	
    static inline void initialise( T& value )
    {
        value = std::make_shared< int >();
    }
    static void uninitialise( T& value )
    {
        value.reset();
    }
	
	static inline void encode( Encoder& encoder, const T& value )
	{
        if( value )
        {
            ::eg::encode< bool >( encoder, true );
            ::eg::encode< int >( encoder, *value );
        }
        else
        {
            ::eg::encode< bool >( encoder, false );
        }
            
	}
	
	static inline void decode( Decoder& decoder, T& value )
	{
        bool bNonNull = false;
		::eg::decode< bool >( decoder, bNonNull );
        if( !bNonNull )
        {
            if( !value )
                value = std::make_shared< int >();
            ::eg::decode< int >( decoder, *value );
        }
        else
        {
            value.reset();
        }
	}
};

}
namespace pybind11
{
    namespace detail
    {
        template <> struct type_caster< std::shared_ptr< int > >
        {
        public:
            PYBIND11_TYPE_CASTER( std::shared_ptr< int >, _("shared_ptr_int"));
        
            bool load( handle src, bool )
            {
                PyObject* pSource = src.ptr();
                PyObject* pTemp = PyNumber_Long( pSource );
                if( pTemp )
                {
                    value = std::make_shared< int >( PyLong_AsLong( pTemp ) );
                    Py_DECREF( pTemp );
                }
                else
                {
                    value.reset();
                }
                return !PyErr_Occurred();
            }
        
            static handle cast( std::shared_ptr< int > src, return_value_policy /* policy */, handle /* parent */)
            {
                if( src )
                {
                    return PyLong_FromLong( *src );
                }
                else
                {
                    Py_INCREF( Py_None );
                    return Py_None;
                }
            }
        };
    }
}
