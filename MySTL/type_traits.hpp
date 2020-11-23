/**
 * 类型萃取
**/
#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_


namespace MySTL {

	struct __true_type {};
	struct __false_type {};

	template<class type>
	struct __type_traits
	{
		using has_trivial_default_constructor =  __false_type ;
		using has_trivial_copy_constructor = __false_type;
		using has_trivial_assignment_constructor = __false_type;
		using has_trivial_destructor =  __false_type;
		using is_POD_type = __false_type;

	};
	template<>
	struct __type_traits<int>
	{
		using has_trivial_default_constructor =  __true_type ;
		using has_trivial_copy_constructor = __true_type;
		using has_trivial_assignment_constructor = __true_type;
		using has_trivial_destructor =  __true_type;
		using is_POD_type = __true_type;
	};
}


#endif