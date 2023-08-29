#pragma once
#include <vector>
#include <array>
#include <ranges>
#include <cassert>
#include <sstream>
#include <string>
#include <functional>
#include <optional>
#include "MenuItem.h"
#include "Enums.h"

namespace KHAS {

	struct WinWidth final { int16_t width; };
	struct WinHeight final { int16_t height; };
	enum class IsFormated { Enable, Disable };
	class Interface
	{
	protected:
		std::vector<std::string> header_;
		std::vector<std::string> data_;
		std::vector<MenuItem> menu_;
		const int16_t k_window_width_;
		const int16_t k_const_width_{ 4 }; // pattern: #_text_#, #_ and _# = 4
		bool default_exit_code_{ true };
	public:
		explicit Interface(WinWidth window_width);

		virtual ~Interface() = default;

		virtual void addHeaderItem(const std::string&, IsFormated, Aligment) noexcept = 0;
		virtual void showHeader() const noexcept = 0;

		virtual void addMenuItem(std::string&&, std::optional<std::function<std::vector<std::string>()>>&&, IsExit) noexcept = 0;
		virtual void addDataItem(std::string&&, IsFormated, Aligment) noexcept = 0;
		virtual void clearData() noexcept = 0;
		virtual void showMenu() const noexcept = 0;

		virtual void loop() const noexcept = 0;

		virtual std::string dividingLine(char) const noexcept = 0;

	protected:

		virtual void setTitle(std::string_view) const noexcept = 0;
		virtual void setWindowSize(int16_t window_width, int16_t window_height) const noexcept = 0;

	};

}