#ifndef GENERATIVE_VEC_INFO_HPP
#define GENERATIVE_VEC_INFO_HPP

#include <type_traits>
#include <glm/glm.hpp>

namespace generative
{
	namespace detail
	{
		template <typename VectorType>
		struct is_vector : std::false_type { };
		template <> struct is_vector<glm::vec2>  : std::true_type { };
		template <> struct is_vector<glm::vec3>  : std::true_type { };
		template <> struct is_vector<glm::vec4>  : std::true_type { };
		template <> struct is_vector<glm::dvec2> : std::true_type { };
		template <> struct is_vector<glm::dvec3> : std::true_type { };
		template <> struct is_vector<glm::dvec4> : std::true_type { };
		
		template <typename VectorType>
		struct is_scalar : std::false_type { };
		template <> struct is_scalar<float>  : std::true_type { };
		template <> struct is_scalar<double> : std::true_type { };
		
		template <typename Type>
		struct assert_vector
		{
			static_assert(is_vector<Type>::value, "Must be a vector type");
		};
		
		template <typename Type>
		struct assert_scalar
		{
			static_assert(is_scalar<Type>::value, "Must be a scalar type");
		};
		
		template <typename Type>
		struct assert_vector_or_scalar
		{
			static_assert(is_vector<Type>::value || is_scalar<Type>::value, "Must be a vector or scalar type");
		};
	}
}

#endif // GENERATIVE_VEC_INFO_HPP

