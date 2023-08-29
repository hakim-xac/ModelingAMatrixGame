#pragma once
#include "Interface.h"
#include "Enums.h"
#include <string>
#include <sstream>
#include <string_view>
#include <iostream>
#include <iomanip>
#include <optional>
#include <limits>
#include <Windows.h>

namespace KHAS {

	template <typename T>
	void PRINT(T&& value, std::ostream& out = std::cout) {
		static int64_t counter{};
		out <<  "c: " << ++counter << " val: " << std::forward<T>(value) << "\n";
	}

	template <typename T>
	[[nodiscard]] T getInput() {
		auto mx = std::numeric_limits<std::streamsize>::max;

		T value{};
		std::cin >> value;
		std::cin.clear();
		std::cin.ignore(mx(), '\n');
		return value;
	}

	template <typename TLeft, typename TRight, typename TValue>
	[[nodiscard]] auto clamp(TLeft&& left, TRight&& right, TValue&& value) {
		return value < left ? left : value > right ? right : value;
	}




	class ConsoleInterface final : public Interface
	{
	private:
		std::ostream& out_;
	public:
		explicit ConsoleInterface(std::string_view title, WinWidth window_width, WinHeight window_height);

		virtual ~ConsoleInterface() {};

		void showHeader() const noexcept override;
		void showMenu() const noexcept override;

		void loop() const noexcept override;

		void addHeaderItem(const std::string& text, IsFormated is_formated = IsFormated::Enable, Aligment aligment = Aligment::CENTER) noexcept override;


		void addDataItem(std::string&& item, IsFormated is_formated = IsFormated::Enable, Aligment aligment = Aligment::CENTER) noexcept;
		void addDataItem(const std::string& item, IsFormated is_formated = IsFormated::Enable, Aligment aligment = Aligment::CENTER) noexcept;
		
		void addMenuItem(std::string&& item, std::optional<std::function<std::vector<std::string>()>>&& func, IsExit type_exit = IsExit::False) noexcept override;
		std::string dividingLine(char c = '.') const noexcept override;

		void clearData() noexcept override;

		template <typename TLString, typename TRString>
			requires std::is_convertible_v<std::string, std::decay_t<TLString>>
		&& std::is_convertible_v<std::string, std::decay_t<TRString>>
			void addHeaderItem(TLString&& left, TRString&& right, Aligment aligment) noexcept;


		template <typename TString>
			requires std::is_convertible_v<std::string, std::decay_t<TString>>
		std::string createString(TString&& text, Aligment aligment = Aligment::CENTER, char del = ' ') const noexcept;

		template <typename TLString, typename TRString>
			requires std::is_convertible_v<std::string, std::decay_t<TLString>>
		&& std::is_convertible_v<std::string, std::decay_t<TRString>>
			std::string createString(TLString&& left, TRString&& right, Aligment aligment = Aligment::CENTER, char del = ' ') const noexcept;


	private:


		void setTitle(std::string_view title) const noexcept override;

		void setWindowSize(int16_t window_width, int16_t window_height) const noexcept override;


		void showData() const noexcept;
		void showData(size_t cmd) const noexcept;

		bool checkCmd(size_t cmd) const noexcept;
		bool checkIndexMenu(size_t index) const noexcept;


		template<typename T>
		[[nodiscard]] T showAndReadInput() const noexcept;
		
		std::string delimiter(char delimiter = '-') const noexcept;

	};






	template <typename TString>
	requires std::is_convertible_v<std::string, std::decay_t<TString>>
	std::string ConsoleInterface::createString(TString&& str, Aligment aligment, char del) const noexcept {
		auto text{ std::forward<TString>(str) };
		int16_t k_text_length{ int16_t(text.size()) };
		int16_t max_length_string{ k_window_width_ - k_const_width_ };

		assert(k_text_length <= max_length_string && "line length limit exceeded!");

		std::ostringstream ss;
		switch (aligment) {
		case Aligment::LEFT:
			ss << "#" << del << std::setfill(del) << std::left << std::setw(max_length_string) << text << del << "#";
			break;

		case Aligment::RIGHT:
			ss << "#" << del << std::setfill(del) << std::right << std::setw(max_length_string) << text << del << "#";
			break;

		case Aligment::CENTER:
		case Aligment::JUSTIFIED:
		default:

			int16_t length_part{ (max_length_string - k_text_length) / int16_t(2) };

			int16_t length_part_left{ length_part + k_text_length };
			int16_t length_part_right{ max_length_string - length_part_left + 1 };

			ss << "#" << del << std::setfill(del) << std::setw(length_part_left) << text
				<< std::setw(length_part_right) << del << "#";
		}
		return ss.str();
	}



	template <typename TLString, typename TRString>
		requires std::is_convertible_v<std::string, std::decay_t<TLString>>
		&& std::is_convertible_v<std::string, std::decay_t<TRString>>
	std::string ConsoleInterface::createString(TLString&& left, TRString&& right, Aligment aligment, char del) const noexcept
		{

		const int16_t k_all_length_strings{ int16_t(left.size()) + int16_t(right.size()) };

		assert((k_all_length_strings + k_const_width_) <= k_window_width_ && "line length limit exceeded!");

		const int16_t k_length_part{ (k_window_width_ - k_const_width_) / int16_t(2) };

		assert(left.size() <= k_length_part && "left.size() - limit exceeded!");
		assert(right.size() <= k_length_part && "left.size() - limit exceeded!");

		int16_t left_size{ int16_t(left.size()) };
		int16_t right_size{ int16_t(right.size()) };

		int16_t length_part_left{ k_length_part };
		int16_t length_part_right{ k_window_width_ - k_length_part - k_const_width_ };


		std::ostringstream ss;
		switch (aligment) {
		case Aligment::LEFT:
			ss << "#" << del << std::setfill(del) << std::left
				<< std::setw(length_part_left) << left
				<< std::setw(length_part_right) << right << del << "#";
			break;

		case Aligment::RIGHT:
			ss << "#" << del << std::setfill(del) << std::right
				<< std::setw(length_part_left) << left
				<< std::setw(length_part_right) << right << del << "#";
			break;

		case Aligment::JUSTIFIED:
			ss << "#" << del << std::setfill(del)
				<< std::left << std::setw(length_part_left) << left
				<< std::right << std::setw(length_part_right) << right << del << "#";
			break;

		case Aligment::CENTER: default:
			int16_t length_part_left_left{ (k_length_part - left_size) / 2 };
			int16_t length_part_left_right{ k_length_part - length_part_left_left - left_size };

			int16_t length_part_right_left{ (k_length_part - right_size) / 2 };
			int16_t length_part_right_right{ k_length_part - length_part_right_left - right_size + 1 };

			ss << "#" << del << std::setfill(del) << std::setw(length_part_left_left + left.size()) << left;
			if (length_part_left_right > 0) ss << std::setw(length_part_left_right) << del;
			ss << std::setw(length_part_right_left + right.size()) << right
				<< std::setw(length_part_right_right) << del << "#";
		}
		return ss.str();
	}


	template <typename T>
	[[nodiscard]] T ConsoleInterface::showAndReadInput() const noexcept
	{
		auto out_handle{ GetStdHandle(STD_OUTPUT_HANDLE) };

		std::string k_input_text{ "¬вод команды:" };
		std::cout << createString(std::move(k_input_text), Aligment::CENTER, '_') << "\n";
		std::cout << delimiter(' ') << "\n";
		std::cout << delimiter('=') << "\n";

		CONSOLE_SCREEN_BUFFER_INFO pc{};
		GetConsoleScreenBufferInfo(out_handle, &pc);
		pc.dwCursorPosition.Y -= 2;
		pc.dwCursorPosition.X = k_window_width_ / 2 - 1;
		SetConsoleCursorPosition(out_handle, pc.dwCursorPosition);

		return  getInput<T>();
	}

	template <typename TLString, typename TRString>
		requires std::is_convertible_v<std::string, std::decay_t<TLString>>
		&& std::is_convertible_v<std::string, std::decay_t<TRString>>
	void ConsoleInterface::addHeaderItem(TLString&& left, TRString&& right, Aligment aligment) noexcept
	{
		header_.emplace_back(createString(std::forward<TLString>(left), std::forward<TRString>(right), aligment, ' '));
	}

}

