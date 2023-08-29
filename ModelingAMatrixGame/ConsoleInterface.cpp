#include "ConsoleInterface.h"
#include <iostream>

namespace KHAS {

	ConsoleInterface::ConsoleInterface(std::string_view title, WinWidth window_width, WinHeight window_height)
		: Interface{ window_width }
		, out_{ std::cout } {

		setTitle(title);
		setWindowSize(window_width.width, window_height.height);
	}

	void ConsoleInterface::showHeader() const noexcept
	{

		std::cout << delimiter('=') << "\n";

		for (auto&& elem : Interface::header_) {
			std::cout << elem << "\n";
		}
		std::cout << delimiter('=') << "\n";
	}

	void ConsoleInterface::showMenu() const noexcept
	{
		using namespace std::string_literals;
		std::cout << delimiter('=') << "\n";
		std::cout << createString("Меню"s, Aligment::CENTER, ' ') << "\n";
		std::cout << delimiter('-') << "\n";
		std::cout << createString("Команды"s, "Действие"s, Aligment::CENTER, ' ') << "\n";
		std::cout << delimiter('.') << "\n";

		for (int32_t i{}; auto && elem : Interface::menu_) {
			std::cout << createString(std::to_string(++i), elem.getName(), Aligment::CENTER, ' ') << "\n";
		}
		if (default_exit_code_) {
			std::cout << delimiter('.') << "\n";
			std::cout << createString(std::to_string(0), "Выход"s, Aligment::CENTER, ' ') << "\n";
		}
		std::cout << delimiter('.') << "\n";
		std::cout << delimiter('=') << "\n";

	}

	void ConsoleInterface::loop() const noexcept
	{
		using namespace std::string_literals;
		
		size_t cmd{};
		showHeader();
		showData();
		showMenu();

		do {
			cmd = showAndReadInput<decltype(cmd)>();
			system("cls");
			showHeader();
			showData();
			showData(cmd);
			showMenu();

		} while (checkCmd(cmd));

		std::cout << "\ngoodbay...\n";
	}

	void ConsoleInterface::addHeaderItem(const std::string& text, IsFormated is_formated, Aligment aligment) noexcept
	{
		header_.emplace_back(is_formated == IsFormated::Enable ? createString(text, aligment, ' ') : text);
	}


	void ConsoleInterface::addDataItem(std::string&& item, IsFormated is_formated, Aligment aligment) noexcept
	{
		data_.emplace_back(is_formated == IsFormated::Enable ? createString(std::move(item), aligment, ' ') : std::move(item));
	}

	void ConsoleInterface::addDataItem(const std::string& item, IsFormated is_formated, Aligment aligment) noexcept
	{
		data_.emplace_back(is_formated == IsFormated::Enable ? createString(item, aligment, ' ') : item);
	}

	void ConsoleInterface::addMenuItem(std::string&& text, std::optional<std::function<std::vector<std::string>()>>&& func, IsExit type_exit) noexcept
	{
		if (type_exit == IsExit::True) default_exit_code_ = false;
		menu_.emplace_back(MenuItem{ std::move(text), std::move(func.value_or([] { return std::vector<std::string> {}; })), type_exit });
	}

	std::string ConsoleInterface::dividingLine(char c) const noexcept
	{
		auto str{ std::string(k_window_width_, c) };
		str.front() = str.back() = '#';
		return str;
	}


	void ConsoleInterface::showData() const noexcept
	{
		for (const auto& elem : data_) {
			//std::cout << createString(elem, Aligment::CENTER, ' ') << "\n";
			std::cout << elem << "\n";
		}
	}

	void ConsoleInterface::showData(size_t cmd) const noexcept
	{
		using namespace std::string_literals;
		if (cmd == 0 || !checkIndexMenu(cmd-1)) return;

		auto data{ menu_[cmd - 1].call() };

		for (const auto& elem : data) {
			std::cout << createString(elem, Aligment::CENTER, '_') << "\n";
		}
				
	}

	bool ConsoleInterface::checkCmd(size_t cmd) const noexcept
	{
		if (default_exit_code_ && cmd == 0) return false;
		if (checkIndexMenu(cmd - 1) && menu_[cmd - 1].typeExit() == IsExit::True) return false;
		//else return menu_[clamp(0, menu_.size()-1, cmd)].typeExit() == IsExit::False;
		//return !default_exit_code_ && (cmd > 0 && menu_.size() > 0 && menu_[cmd - 1].typeExit() != IsExit::True || cmd == 0);
		return true;
	}

	bool ConsoleInterface::checkIndexMenu(size_t index) const noexcept
	{
		return index >= 0 && index < menu_.size();
	}

	void ConsoleInterface::setTitle(std::string_view title) const noexcept
	{
		SetConsoleTitleA(title.data());
	}

	void ConsoleInterface::setWindowSize(int16_t window_width, int16_t window_height) const noexcept 
	{

		auto out_handle{ GetStdHandle(STD_OUTPUT_HANDLE) };
		SMALL_RECT srect{ 0, 0, window_width , window_height };

		COORD coord{ window_width + 1 , window_height + 1 };
		SetConsoleWindowInfo(out_handle, true, &srect);
		SetConsoleScreenBufferSize(out_handle, std::move(coord));
	}

	void ConsoleInterface::clearData() noexcept
	{
		data_.clear();
	}

	std::string ConsoleInterface::delimiter(char delimiter) const noexcept 
	{
		std::string str(k_window_width_, delimiter);
		str.front() = str.back() = '#';
		return str;
	}

}