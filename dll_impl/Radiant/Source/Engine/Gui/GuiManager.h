
namespace rdt {

#define GUI_DEFAULT_FONT 0
#define GUI_DEFAULT_FONT_SIZE 12

	class GuiManager {
	private:
		GuiManager();
		~GuiManager();

		static GuiManager* m_instance;

		struct FontCache {
			std::string filepath;
			std::unordered_map<unsigned int, ImFont*> font_ptrs;
		};

		std::unordered_map<int, FontCache> m_fonts;

	public:
		static GuiManager* GetInstance();

		static void Initialize();
		static void Destroy();

		static void LoadFont(int magicWord, const std::string& filepath) { m_instance->LoadFontImpl(magicWord, filepath); }

		static ImFont* GetFont(const int magicWord, const unsigned int fontSize) { return m_instance->GetFontImpl(magicWord, fontSize); }
	private:
		void LoadFontImpl(int magicWord, const std::string& filepath);

		ImFont* GetFontImpl(const int magicWord, unsigned int fontSize);
	};
}