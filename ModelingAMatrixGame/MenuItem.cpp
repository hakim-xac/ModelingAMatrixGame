#include "MenuItem.h"

namespace KHAS {

	MenuItem::MenuItem(std::string&& name, std::function<std::vector<std::string>()>&& func, IsExit type_exit)
		:name_{ std::move(name) }
		, func_(std::move(func))
		, type_exit_{ type_exit } {}

	std::string MenuItem::getName() const noexcept
	{
		return name_;
	}

	std::vector<std::string> MenuItem::call() const noexcept
	{
		return func_();
	}

	IsExit MenuItem::typeExit() const noexcept
	{
		return type_exit_;
	}
	
}