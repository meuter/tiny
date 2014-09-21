#ifndef __TINY_RENDERING_GL_HANDLE___
#define __TINY_RENDERING_GL_HANDLE___

namespace tiny { namespace rendering { namespace gl {

	template<typename T, void destroy(T)>
	class Handle
	{
		using Ref      = T(&);
		using ConstRef = const T(&);

	public:
		Handle(T value = 0) : value(value) {}
		Handle(const Handle &other) = delete;
		Handle(Handle &&other) : value(other) {	other.value = 0; }

		~Handle() { if (value) destroy(value); }

		Handle &operator=(const Handle &other) = delete;

		Handle &operator=(Handle &&other) 
		{ 
			if (value) destroy(value);
			value = other; 
			other.value = 0;
			return (*this); 
		}

		inline T *operator &()	  	      { return &value; }
		inline const T *operator&() const { return &value; }
		inline operator Ref()             { return value; }
		inline operator ConstRef() const  { return value; }

	private:
		T value;
	};

}}}



#endif