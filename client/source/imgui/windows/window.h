#pragma once



class window
{
	public:
		window()
		{
			m_open_key = ImGuiKey_F8;
			m_is_open = true;
		}

		~window()
		{

		}

		void init()
		{

		}

		void render()
		{
			if (m_open_key != ImGuiKey_None)
			{
				if (ImGui::IsKeyPressed(m_open_key))
					m_is_open ^= true;
			}

			if (m_is_open)
				ImGui::ShowDemoWindow();
		}

		bool is_open() const
		{
			return m_is_open;
		}

	protected:
		ImGuiKey m_open_key;

		ImVec2 m_position;
		ImVec2 m_size;

	private:
		bool m_is_open;
};