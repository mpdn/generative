#ifndef GENERATIVE_VEC_INFO_HPP
#define GENERATIVE_VEC_INFO_HPP

#include <type_traits>
#include <glm/glm.hpp>

namespace generative
{
	namespace detail
	{
		template <typename VectorType>
		struct length
		{
			//static_assert(false, "Length can only be used with glm vector types");
		};
	
		template <typename ScalarType>
		struct length<glm::detail::tvec1<ScalarType>> : std::integral_constant<int, 1> { };
		template <typename ScalarType>
		struct length<glm::detail::tvec2<ScalarType>> : std::integral_constant<int, 2> { };
		template <typename ScalarType>
		struct length<glm::detail::tvec3<ScalarType>> : std::integral_constant<int, 3> { };
		template <typename ScalarType>
		struct length<glm::detail::tvec4<ScalarType>> : std::integral_constant<int, 4> { };
	
		template <typename VectorType>
		struct is_vector : std::false_type { };
		template <typename ScalarType>
		struct is_vector<glm::detail::tvec1<ScalarType>> : std::true_type { };
		template <typename ScalarType>
		struct is_vector<glm::detail::tvec2<ScalarType>> : std::true_type { };
		template <typename ScalarType>
		struct is_vector<glm::detail::tvec3<ScalarType>> : std::true_type { };
		template <typename ScalarType>
		struct is_vector<glm::detail::tvec4<ScalarType>> : std::true_type { };
	
		template <typename Type>
		struct assert_vector
		{
			static_assert(is_vector<Type>::value, "Must be a vector type");
		};
	}
}

#endif // GENERATIVE_VEC_INFO_HPP

