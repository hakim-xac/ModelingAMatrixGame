#pragma once
#include "ConsoleInterface.h"
#include <vector>
#include <string>
#include <optional>
#include <random>
#include <type_traits>

namespace KHAS {

	struct DataInput final { 
		std::string title;
		size_t number_of_unknown_variables;
		size_t number_of_parties;
	};

	struct Coord final { size_t col; size_t row; };
	struct PairCoords final { Coord first; Coord second; };

	enum class MatrixAlignment { Horizontal, Vertical };
	class Solution final
	{
	private:
		std::string window_name_;
		const std::string filename_;
		ConsoleInterface app_;
		std::vector<std::vector<int32_t>> data_;
		size_t number_of_unknown_variables_;
		size_t number_of_parties_;
	public:
		explicit Solution(DataInput input, const std::string& filename) noexcept;

		void start() noexcept;

	private:

		template <typename T>
		bool load(const std::string& filename) noexcept;

		void showError(std::string&& str) noexcept;

		void printDataInTheInterface(const std::vector<std::vector<int32_t>>& data, const std::string& iter_header) noexcept;
		
		void printCoefficient(std::pair<double, double> coof, std::string&& variable_name) noexcept;
		
		bool modelingAMatrixGame(const std::vector<std::vector<int32_t>>& actual_data) noexcept;
		
		[[nodiscard]] double calculatePrice(const std::vector<std::vector<int32_t>>& data, PairCoords coord, std::pair<double, double> coof) const noexcept;
		
		void printCalculatePrice(double price) noexcept;

		double generateRandom() const noexcept;


		void printOptimalStrategy(const std::vector<std::vector<int32_t>>& data
			, std::string&& player_name, std::string&& variable_name
			, MatrixAlignment ma, const PairCoords& coord) noexcept;


		[[nodiscard]] std::pair<double, double> coefficient—alculation(const std::vector<std::vector<int32_t>>& data
			, PairCoords coof_left, PairCoords coof_right) const noexcept;

		template <typename T>
			requires std::is_fundamental_v<std::decay_t<T>>
		std::pair<T, T> getValueFromData(const std::vector<std::vector<T>>& data, const PairCoords& di_coord) const noexcept;

		template <typename T>
		requires std::is_arithmetic_v<T>
		T randFromRange(T lhs, T rhs) const noexcept;


	private:
		void init() noexcept;
	};



	template <typename T>
	bool Solution::load(const std::string& filename) noexcept
	{
		std::ifstream input_file{ filename, std::ios::in };
		if (!input_file.is_open()) return false;


		std::vector<T> tmp_data;
		tmp_data.reserve(number_of_unknown_variables_);

		for (size_t i{};; ++i) {
			T tmp{};
			if (!(input_file >> tmp)) break;

			tmp_data.emplace_back(tmp);

			if ((i + 1) % number_of_unknown_variables_ == 0) {
				data_.emplace_back(tmp_data);
				tmp_data.clear();
			}
		}

		return true;
	}



	template <typename T>
		requires std::is_fundamental_v<std::decay_t<T>>
	std::pair<T, T> Solution::getValueFromData(const std::vector<std::vector<T>>& data, const PairCoords& di_coord) const noexcept
	{
		auto [first_col, first_row] { di_coord.first };
		auto [second_col, second_row] { di_coord.second };

		return { data[first_row][first_col], data[second_row][second_col] };
	}

	template <typename T>
		requires std::is_arithmetic_v<T>
	T Solution::randFromRange(T lhs, T rhs) const noexcept
	{
		std::mt19937 gen{ std::random_device{}() };

		if constexpr (std::is_floating_point_v<T>) {
			std::uniform_real_distribution<> dist(lhs, rhs);
			return dist(gen);
		}
		else
		{ 
			std::uniform_int_distribution<> dist(lhs, rhs);
			return dist(gen);
		}
	}
}