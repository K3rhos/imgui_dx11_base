#pragma once



template<typename T>
class events_handler
{
	public:
		events_handler() = default;
		~events_handler()
		{
			m_events.clear();
		}

		void register_event(const std::string& _name, std::function<void(T*)> _function)
		{
			m_events.emplace(_name, _function);
		}


		void call_event(const std::string& _name, T* _self = nullptr)
		{
			if (m_events.contains(_name))
			{
				m_events[_name](_self);
			}
		}

	private:
		std::unordered_map<std::string, std::function<void(T*)>> m_events;
};