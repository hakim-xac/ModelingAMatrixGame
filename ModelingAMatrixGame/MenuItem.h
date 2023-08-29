#pragma once

#include "Enums.h"
#include <string>
#include <vector>
#include <functional>

namespace KHAS {
	class MenuItem final
	{
	public:
		explicit MenuItem(std::string&& name, std::function<std::vector<std::string>()>&& func, IsExit type_exit);
		std::string getName() const noexcept;
		std::vector<std::string> call() const noexcept;
		IsExit typeExit() const noexcept;
	private:
		std::string name_;
		std::function<std::vector<std::string>()> func_;
		IsExit type_exit_;
	};

}