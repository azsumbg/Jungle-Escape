#pragma once

#ifdef JUNGLE_EXPORTS
#define JUNGLE_API __declspec(dllexport)
#else
#define JUNGLE_API __declspec(dllimport)
#endif

#include <random>
#include <type_traits>

constexpr float scr_width{ 900.0f };
constexpr float scr_height{ 600.0f };

constexpr float sky{ 50.0f };
constexpr float ground{ 550.0f };

constexpr int BAG_BAD_PTR{ 666 };
constexpr int BAG_BAD_INDEX{ 667 };
constexpr int BAG_BAD_ARG{ 668 };
constexpr int BAG_BAD_ERR{ 669 };
constexpr int BAG_NO_ELEMENTS{ 670 };

constexpr char RUN{ 0b00000000 };
constexpr char JUMP_UP{ 0b00000010 };
constexpr char JUMP_DOWN{ 0b00000100 };
constexpr char FALLING{ 0b00001000 };
constexpr char STOP{ 0b00010000 };
constexpr char SHOOT{ 0b00100000 };

enum class dirs { right = 0, left = 1, up = 2, down = 3, stop = 4 };
enum class tiles {
	dirt = 0, water = 1, dirt_water = 2, trap_axe = 3, trap_bolt = 4, trap_spear = 5, tree1 = 6,
	tree2 = 7, tree3 = 8
};
enum class platforms { flat_platform1 = 0, flat_platform2 = 1, high_platform = 2 };
enum class shots { arrow = 0, tomahawk = 1 };
enum class evils { flyer = 0, mushroom = 1, snail = 2, octopus = 3 };
enum class assets { potion = 0, gold = 1, crystal = 2, chest = 3 };

struct FPOINT
{
	float x{ 0 };
	float y{ 0 };
};
struct FRECT
{
	float left { 0 };
	float up{ 0 };
	float right{ 0 };
	float down{ 0 };
};

namespace dll
{
	/// TEMPLATES ***************************************
	
	class JUNGLE_API EXCEPTION;

	template<typename T> class BAG
	{
	private:
		T* m_ptr{ nullptr };
		size_t m_size{ 1 };
		size_t next_pos{ 0 };
		

	public:
		BAG() :m_ptr{ reinterpret_cast<T*>(calloc(1, sizeof(T))) } {};
		BAG(size_t capacity) :m_size{ capacity }, m_ptr{ reinterpret_cast<T*>(calloc(capacity, sizeof(T))) } {};
		BAG(BAG& other)
		{
			if (!other.m_ptr)throw EXCEPTION(BAG_BAD_ARG);

			m_size = other.m_size;
			m_ptr = reinterpret_cast<T*>(calloc(m_size, sizeof(T)));

			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			else if (m_size > 0)
			{
				for (size_t i = 0; i < m_size; ++i)m_ptr[i] = other.m_ptr[i];
			}
		}
		BAG(BAG&& other)
		{
			if (!other.m_ptr)throw EXCEPTION(BAG_BAD_ARG);

			m_size = other.m_size;
			m_ptr = other.m_ptr;

			other.m_ptr = nullptr;
		}

		~BAG()
		{
			free(m_ptr);
		}

		BAG& operator=(BAG& other)
		{
			if (!other.m_ptr)throw EXCEPTION(BAG_BAD_ARG);
			if (m_ptr == other.m_ptr)throw EXCEPTION(BAG_BAD_ARG);

			free(m_ptr);

			m_size = other.m_size;
			m_ptr = reinterpret_cast<T*>(calloc(m_size, sizeof(T)));

			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			else if (m_size > 0)
			{
				for (size_t i = 0; i < m_size; ++i)m_ptr[i] = other.m_ptr[i];
			}

			return *this;
		}
		BAG& operator=(BAG&& other)
		{
			if (!other.m_ptr)throw EXCEPTION(BAG_BAD_ARG);

			free(m_ptr);
			m_size = other.m_size;
			m_ptr = other.m_ptr;
			other.m_ptr = nullptr;
			
			return *this;
		}

		T& operator[](size_t index)
		{
			if (index >= next_pos)throw EXCEPTION(BAG_BAD_INDEX);
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			if (next_pos == 0)throw EXCEPTION(BAG_NO_ELEMENTS);

			return m_ptr[index];
		}

		T& front() const
		{
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			if (next_pos == 0)throw EXCEPTION(BAG_NO_ELEMENTS);

			return *m_ptr;
		}
		T& back() const
		{
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			if (next_pos == 0)throw EXCEPTION(BAG_NO_ELEMENTS);

			return m_ptr[next_pos - 1];
		}

		size_t capacity()const
		{
			return m_size;
		}
		size_t size() const
		{
			return next_pos;
		}
		bool empty() const
		{
			return (next_pos == 0);
		}

		void clear()
		{
			free(m_ptr);
			m_size = 1;
			next_pos = 0;

			m_ptr = reinterpret_cast<T*>(calloc(m_size, sizeof(T)));
		}

		void push_back(T element)
		{
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			else
			{
				if (next_pos + 1 <= m_size)
				{
					m_ptr[next_pos] = element;
					++next_pos;
				}
				else
				{
					++m_size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, sizeof(T) * m_size));
					if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
					else
					{
						m_ptr[next_pos] = element;
						++next_pos;
					}
				}
			}
		}
		void push_back(T* element)
		{
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			else
			{
				if (next_pos + 1 <= m_size)
				{
					m_ptr[next_pos] = *element;
					++next_pos;
				}
				else
				{
					++m_size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, sizeof(T) * m_size));
					if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
					else
					{
						m_ptr[next_pos] = *element;
						++next_pos;
					}
				}
			}
		}

		void push_front(T element)
		{
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			else
			{
				if (next_pos == 0)
				{
					*m_ptr = element;
					++next_pos;
				}
				else
				{
					if (next_pos + 1 < m_size)
					{
						for (size_t next_index = next_pos; next_index >= 1; --next_index)
						{
							m_ptr[next_index] = m_ptr[next_index - 1];
						}
						++next_pos;
						*m_ptr = element;
					}
					else
					{
						++m_size;
						m_ptr = reinterpret_cast<T*>(realloc(m_ptr, sizeof(T) * m_size));
						if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
						else
						{
							for (size_t next_index = next_pos; next_index >= 1; --next_index)
							{
								m_ptr[next_index] = m_ptr[next_index - 1];
							}
						}
						++next_pos;
						*m_ptr = element;
					}
				}
			}
		}
		void push_front(T* element)
		{
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			else
			{
				if (next_pos == 0)
				{
					*m_ptr = *element;
					++next_pos;
				}
				else
				{
					if (next_pos + 1 < m_size)
					{
						for (size_t next_index = next_pos; next_index >= 1; --next_index)
						{
							m_ptr[next_index] = m_ptr[next_index - 1];
						}
						++next_pos;
						*m_ptr = *element;
					}
					else
					{
						++m_size;
						m_ptr = reinterpret_cast<T*>(realloc(m_ptr, sizeof(T) * m_size));
						if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
						else
						{
							for (size_t next_index = next_pos; next_index >= 1; --next_index)
							{
								m_ptr[next_index] = m_ptr[next_index - 1];
							}
						}
						++next_pos;
						*m_ptr = *element;
					}
				}
			}
		}

		void insert(size_t index, T element)
		{
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			if (index >= next_pos)throw EXCEPTION(BAG_BAD_INDEX);

			if (next_pos == 0)
			{
				*m_ptr = element;
			}
			else
			{
				if (next_pos + 1 < m_size)
				{
					for (size_t count = next_pos; count >= index; --count)m_ptr[count] = m_ptr[count - 1];
					m_ptr[index] = element;
					++next_pos;
				}
				else
				{
					++m_size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, sizeof(T) * m_size));
					if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
					else
					{
						for (size_t count = next_pos; count >= index; --count)m_ptr[count] = m_ptr[count - 1];
						m_ptr[index] = element;
						++next_pos;
					}
				}
			}
		}
		void insert(size_t index, T* element)
		{
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			if (index >= next_pos)throw EXCEPTION(BAG_BAD_INDEX);

			if (next_pos == 0)
			{
				*m_ptr = *element;
			}
			else
			{
				if (next_pos + 1 < m_size)
				{
					for (size_t count = next_pos; count >= index; --count)m_ptr[count] = m_ptr[count - 1];
					m_ptr[index] = *element;
					++next_pos;
				}
				else
				{
					++m_size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, sizeof(T) * m_size));
					if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
					else
					{
						for (size_t count = next_pos; count >= index; --count)m_ptr[count] = m_ptr[count - 1];
						m_ptr[index] = *element;
						++next_pos;
					}
				}
			}
		}

		void remove(size_t index)
		{
			if (index >= next_pos)throw EXCEPTION(BAG_BAD_INDEX);
			if (!m_ptr)throw EXCEPTION(BAG_BAD_PTR);
			else
			{
				for (size_t count = index; count <= next_pos - 1; ++count)
				{
					m_ptr[count] = m_ptr[count + 1];
				}
			}

			--next_pos;
		}
	};

	template<typename T>concept IsPrimitive = std::is_same<char, T>::value || std::is_same<short, T>::value
		|| std::is_same<wchar_t, T>::value || std::is_same<int, T>::value || std::is_same<float, T>::value
		|| std::is_same<double, T>::value || std::is_same<long, T>::value;

	template<IsPrimitive T>void PrimeSort(BAG<T>& bag, bool ascending = true)
	{
		if (bag.empty())throw EXCEPTION(BAG_NO_ELEMENTS);

		if (ascending)
		{
			bool ok = false;

			while (!ok)
			{
				ok = true;

				for (size_t i = 0; i < bag.size() - 1; ++i)
				{
					if (bag[i] > bag[i + 1])
					{
						T temp = bag[i];
						bag[i] = bag[i + 1];
						bag[i + 1] = temp;
						ok = false;
					}
				}
			}
		}
		else
		{
			bool ok = false;

			while (!ok)
			{
				ok = true;

				for (size_t i = 0; i < bag.size() - 1; ++i)
				{
					if (bag[i] < bag[i + 1])
					{
						T temp = bag[i];
						bag[i] = bag[i + 1];
						bag[i + 1] = temp;
						ok = false;
					}
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////
	
	class JUNGLE_API RANDIT
	{
	private:
		std::mt19937* twister{ nullptr };

	public:
		RANDIT();
		~RANDIT();

		int operator()(int min, int max);
		float operator()(float min, float max);
	};

	class JUNGLE_API EXCEPTION
	{
	private:
		int _err_code{ 0 };

	public:
		EXCEPTION(int which_err);

		const char* get() const;
	};

	class JUNGLE_API PROTON
	{
	protected:
		float _width{ 0 };
		float _height{ 0 };

		bool _in_heap = false;

	public:
		FPOINT start{};
		FPOINT end{};
		FPOINT center{};
		float x_rad{ 0 };
		float y_rad{ 0 };

		PROTON();
		PROTON(float _sx, float _sy);
		PROTON(float _sx, float _sy, float _s_width, float _s_height);

		virtual ~PROTON() {};

		float get_width() const;
		float get_height() const;

		void set_width(float new_width);
		void set_height(float new_height);
		void set_edges();
		void new_dims(float new_width, float new_height);

		bool in_heap() const;

		static PROTON* create(float sx, float sy, float s_width, float s_height);
	};

	class JUNGLE_API TILE :public PROTON
	{
	private:
		float _speed{ 1.0f };

		TILE(tiles _type, float _where_x, float _where_y, dirs _dir);

	public:
		tiles type{ tiles::dirt };
		dirs dir = dirs::stop;

		bool move(dirs to_where, float gear);

		void Release();

		static TILE* create(tiles type, float where_x, float where_y, dirs dir);
	};

	class JUNGLE_API PLATFORM :public PROTON
	{
	private:
		float _speed{ 1.0f };

		PLATFORM(platforms _type, float _where_x, float _where_y, dirs _dir);

	public:
		platforms type{ platforms::flat_platform1 };
		dirs dir = dirs::stop;

		bool move(dirs to_where, float gear);

		void Release();

		static PLATFORM* create(platforms type, float where_x, float where_y, dirs dir);
	};

	class JUNGLE_API ASSET :public PROTON
	{
	private:
		float _speed{ 1.0f };

		ASSET(assets _type, float _where_x, float _where_y, dirs _dir);

	public:
		assets type{ assets::gold };
		dirs dir = dirs::stop;

		bool move(dirs to_where, float gear);

		void Release();

		static ASSET* create(assets type, float where_x, float where_y, dirs dir);
	};

	class JUNGLE_API HERO :public PROTON
	{
	private:
		float _speed = 0.1f;

		float jump_sx{ 0 };
		float jump_ex{ 0 };
		float jump_sy{ 0 };
		float jump_ey{ 0 };

		int frame{ 0 };
		int frame_delay{ 15 };
		
		HERO(float _sx, float _sy);

	public:
		dirs dir{ dirs::stop };
		char state{ RUN };
		
		bool in_jump = false;
		bool on_platform{ false };

		FRECT platform{};

		int lifes{ 100 };
		int strenght{ 50 };
		int armor{ 1 };

		void move(float gear);
		void jump(float gear);
		void set_platform(FRECT current_platform);
		void fall(float gear);
		
		int get_frame();

		void Release();

		static HERO* create(float sx, float sy);
	};

	class JUNGLE_API SHOT :public PROTON
	{
	private:
		float move_sx{ 0 };
		float move_sy{ 0 };
		float move_ex{ 0 };
		float move_ey{ 0 };
		float slope{ 0 };
		float intercept{ 0 };

		float _speed{ 0 };

		bool ver_dir{ 0 };
		bool hor_dir{ 0 };

		void set_path(float _end_x, float _end_y);

		SHOT(shots _type, float _start_x, float _start_y, float _target_x, float _target_y);

	public:
		shots type{ shots::tomahawk };
		dirs dir{ dirs::stop };

		int damage{ 0 };

		bool move(float gear);
		static SHOT* create(shots type, float start_x, float start_y, float target_x, float target_y);
		void Release();
	};

	class JUNGLE_API EVIL :public PROTON
	{
	private:
		float move_sx{ 0 };
		float move_sy{ 0 };
		float move_ex{ 0 };
		float move_ey{ 0 };
		float slope{ 0 };
		float intercept{ 0 };

		float jump_sx{ 0 };
		float jump_ex{ 0 };
		float jump_sy{ 0 };
		float jump_ey{ 0 };

		float _speed{ 0 };

		bool ver_dir{ 0 };
		bool hor_dir{ 0 };

		int max_frames{ 0 };
		int frame_delay{ 0 };
		int max_frame_delay{ 0 };
		int frame{ 0 };

		int attack_delay = 0;
		int max_attack_delay = 0;

		RANDIT _rand{};
		
		void set_path(float _end_x, float _end_y);

		EVIL(evils _type, float _sx, float _sy);

	public:
		evils type{ evils::flyer };
		dirs dir{ dirs::stop };
		char state{ RUN };

		int lifes{ 0 };
		int damage{ 0 };
		bool in_jump{ false };
		bool on_platform{ false };

		FRECT platform{};


		bool move(float gear);
		void jump(float gear);
		void set_platform(FRECT current_platform);
		void fall(float gear);
		
		int get_frame();

		int attack();

		void Release();

		static EVIL* create(evils type, float start_x, float start_y);

		friend char JUNGLE_API AIDispatcher(EVIL& evil, FPOINT hero_center, BAG<FPOINT>& tomahawks, BAG<FPOINT>& obstacles);
	};
	
	// FUNCTIONS *************************************

	JUNGLE_API float Distance(FPOINT first, FPOINT second);

	JUNGLE_API void Sort(BAG<FPOINT>& bag, FPOINT criterion, bool ascending = true);

	JUNGLE_API bool Intersect(FRECT first, FRECT second);

	JUNGLE_API bool Intersect(FPOINT first, FPOINT second, float x_rad1, float x_rad2, float y_rad1, float y_rad2);

	char JUNGLE_API AIDispatcher(EVIL& evil, FPOINT hero_center, BAG<FPOINT>& tomahawks, BAG<FPOINT>& obstacles);
}