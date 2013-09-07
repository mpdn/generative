#ifndef COHERENT_BIND_LEFT_HPP
#define COHERENT_BIND_LEFT_HPP

namespace coherent
{
	namespace detail
	{
		namespace
		{
			/// Functor that binds a single value to the left side of a function
			template <typename FunctionType, typename BoundType>
			class bind_left_value
			{
				private:
				FunctionType function;
				BoundType bound;
				
				public:
				bind_left_value(FunctionType pfunction, BoundType pbound)
					: function(pfunction), bound(pbound)
				{ }
				
				template <typename... ParamTypes>
				auto operator() (ParamTypes... params) ->
				decltype(function(bound, params...))
				{
					return function(bound, params...);
				}
			};
		}
	
		template <typename FunctionType, typename BoundType>
		bind_left_value<FunctionType, BoundType>
		bind_left(FunctionType function, BoundType bound)
		{
			return bind_left_value<FunctionType, BoundType>(function, bound);
		}
	
		template <typename FunctionType, typename BoundType, typename... BoundTypes>
		auto bind_left(FunctionType function, BoundType bound, BoundTypes... bounds) ->
		decltype(bind_left(bind_left(function, bound), bounds...))
		{
			return bind_left(bind_left(function, bound), bounds...);
		}
	}
}

#endif // COHERENT_BIND_LEFT_HPP

