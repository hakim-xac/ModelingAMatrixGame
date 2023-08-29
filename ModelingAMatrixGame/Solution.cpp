#include "Solution.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>

namespace KHAS {
	Solution::Solution(DataInput input, const std::string& filename) noexcept
		: window_name_{ input.title }
		, filename_{ filename }
		, app_{ window_name_ , WinWidth{ .width = 80 }, WinHeight{ .height = 100 } }
		, data_{}
		, number_of_unknown_variables_{ input.number_of_unknown_variables }
		, number_of_parties_{ input.number_of_parties }
	{
		init();
	}

	void Solution::init() noexcept
	{
		using namespace std::string_literals;

		app_.addHeaderItem(window_name_);
		app_.addHeaderItem(app_.dividingLine(), IsFormated::Disable);
		app_.addHeaderItem("Лабораторная работа:"s, "№ 2"s, Aligment::JUSTIFIED);
		app_.addHeaderItem("Группа:"s, "ПБ-11"s, Aligment::JUSTIFIED);
		app_.addHeaderItem("Студент:"s, "Хакимов А.С."s, Aligment::JUSTIFIED);
		auto pos{ filename_.rfind('/') };
		if (pos == std::string::npos) pos = filename_.rfind('\\');
		if (pos != std::string::npos)
			app_.addHeaderItem("Данные из файла:"s, "\"" + std::string(filename_.begin() + pos + 1, filename_.end()) + "\"", Aligment::JUSTIFIED);

	}

	void Solution::start() noexcept
	{

		if (!load<int32_t>(filename_)) {
			showError("Ошибка загрузки данных из файла!");			
		}

		if (!modelingAMatrixGame(data_)) {
			showError("Ошибка формирования данных!");
		}
		//window.addMenuItem("hi", [] { return std::vector<std::string>{ {"qwerty"} };  }, IsExit::True);
		//window.addMenuItem("fgfgfgfghfghf:", std::nullopt);
		//window.addMenuItem("Выход:", std::nullopt );
		app_.loop();
	}

	void Solution::showError(std::string&& str) noexcept
	{
		data_.clear();
		app_.clearData();
		app_.addDataItem(std::move(str));
	}

	void Solution::printDataInTheInterface(const std::vector<std::vector<int32_t>>& data, const std::string& iter_header) noexcept
	{

		app_.addDataItem(app_.dividingLine(), IsFormated::Disable);
		app_.addDataItem(iter_header);		
		app_.addDataItem(app_.dividingLine(), IsFormated::Disable);


		for (auto&& row: data) {
			std::ostringstream ss;
			for (auto&& col: row) {
				ss << std::fixed << std::setprecision(4) << std::setw(11) << col << " ";
			}
			app_.addDataItem(ss.str());
		}
	}

	void Solution::printCoefficient(std::pair<double, double> coof, std::string&& variable_name) noexcept
	{
		app_.addDataItem(app_.dividingLine(), IsFormated::Disable);
		std::string str{ variable_name+" = ("+ std::to_string(coof.first) +", " + std::to_string(coof.second) + ")"};

		app_.addDataItem(std::move(str));
		app_.addDataItem(app_.dividingLine(), IsFormated::Disable);
	}

	bool Solution::modelingAMatrixGame(const std::vector<std::vector<int32_t>>& actual_data) noexcept
	{

		if (actual_data.size() != 2) return false;

		printDataInTheInterface(actual_data, "Начальные данные");
		app_.addDataItem(app_.dividingLine(), IsFormated::Disable);

		auto b1{ PairCoords{.first{.col = 0, .row = 0 }, .second{.col = 0, .row = 1 } } };
		auto b2{ PairCoords{.first{.col = 1, .row = 0 }, .second{.col = 1, .row = 1 } } };

		auto a1{ PairCoords{.first{.col = 0, .row = 0 }, .second{.col = 1, .row = 0 } } };
		auto a2{ PairCoords{.first{.col = 0, .row = 1 }, .second{.col = 1, .row = 1 } } };

		printOptimalStrategy(actual_data, "B1", "p", MatrixAlignment::Horizontal, b1);
		printOptimalStrategy(actual_data, "B2", "p", MatrixAlignment::Horizontal, b2);

		app_.addDataItem(app_.dividingLine(), IsFormated::Disable);


		printOptimalStrategy(actual_data, "A1", "q", MatrixAlignment::Horizontal, a1);
		printOptimalStrategy(actual_data, "A2", "q", MatrixAlignment::Horizontal, a2);

		auto p{ coefficientСalculation(actual_data, b1, b2) };
		auto q{ coefficientСalculation(actual_data, a1, a2) };

		printCoefficient(p, "p");
		printCoefficient(q, "q");

		auto price{ calculatePrice(actual_data, b1, p) };

		printCalculatePrice(price);

		//startGameAlias(actual_data, std::make_pair(p.first, q.first), price);
		using namespace std::literals;
		app_.addDataItem(app_.dividingLine('='), IsFormated::Disable);

		double all_prize{};
		size_t counter_af{};
		size_t counter_bf{};

		for (size_t i{}, ie{ number_of_parties_ }; i != ie; ++i) {

			auto rndA{ generateRandom() };
			auto rndB{ generateRandom() };

			app_.addDataItem(app_.dividingLine('-'), IsFormated::Disable);
			app_.addDataItem(app_.createString("Итерация № "s, std::to_string(i + 1)), IsFormated::Disable);
			app_.addDataItem(app_.dividingLine(), IsFormated::Disable);
			app_.addDataItem(app_.createString("Случайное число игрока А: "+std::to_string(rndA),
				"Случайное число игрока B: " + std::to_string(rndB),
				Aligment::JUSTIFIED), IsFormated::Disable);

			counter_af += rndB < p.first;
			counter_bf += rndA < q.first;

			app_.addDataItem(app_.createString("Стратегия игрока A: "s + (rndB < p.first ? "A1"s : "A2"s),
				"Стратегия игрока B: "s+(rndA < q.first ? "B1"s : "B2"s),
				Aligment::JUSTIFIED), IsFormated::Disable);
			app_.addDataItem(app_.dividingLine(), IsFormated::Disable);

			auto prize{ actual_data[!(rndB < p.first)][!(rndA < q.first)] };
			all_prize += prize;
			app_.addDataItem(app_.createString("Выйгрыш A"s, std::to_string(prize), Aligment::JUSTIFIED), IsFormated::Disable);
			app_.addDataItem(app_.createString("Накопленный Выйгрыш A"s, std::to_string(all_prize), Aligment::JUSTIFIED), IsFormated::Disable);
			app_.addDataItem(app_.createString("Средний Выйгрыш A"s, std::to_string(all_prize / (i+1)), Aligment::JUSTIFIED), IsFormated::Disable);
			app_.addDataItem(app_.dividingLine(), IsFormated::Disable);
		}
		app_.addDataItem(app_.dividingLine('='), IsFormated::Disable);

		app_.addDataItem("Итоги игры"s);
		app_.addDataItem(app_.dividingLine('-'), IsFormated::Disable);
		app_.addDataItem(app_.createString("Средний Выйгрыш A"s, std::to_string(all_prize / number_of_parties_), Aligment::JUSTIFIED), IsFormated::Disable);
		app_.addDataItem(app_.createString("Количество партий"s, std::to_string(number_of_parties_), Aligment::JUSTIFIED), IsFormated::Disable);
		app_.addDataItem(app_.createString("Количество стратегий A1: "s + std::to_string(counter_af),
			"Количество стратегий A2: "s + std::to_string(number_of_parties_ - counter_af),
			Aligment::JUSTIFIED), IsFormated::Disable);
		app_.addDataItem(app_.createString("Количество стратегий B1: "s + std::to_string(counter_bf),
			"Количество стратегий B2: "s + std::to_string(number_of_parties_ - counter_bf),
			Aligment::JUSTIFIED), IsFormated::Disable);
		app_.addDataItem(app_.createString("-p: ("s + std::to_string(1.0*counter_af / number_of_parties_) + ";"s + std::to_string(1.0 * (number_of_parties_-counter_af) / number_of_parties_) + ")"s,
			"-q: ("s + std::to_string(1.0 * counter_bf / number_of_parties_) + ";"s + std::to_string(1.0 * (number_of_parties_ - counter_bf) / number_of_parties_) + ")"s,
			Aligment::JUSTIFIED), IsFormated::Disable);
		app_.addDataItem(app_.dividingLine(), IsFormated::Disable);

		return true;
	}

	[[nodiscard]] double Solution::calculatePrice(const std::vector<std::vector<int32_t>>& data, PairCoords coord, std::pair<double, double> coof) const noexcept
	{
		assert(data.size() == 2 && "Incorrect data!");
		return coof.first * data[coord.first.row][coord.first.col] + coof.second * data[coord.second.row][coord.second.col];
	}

	void Solution::printCalculatePrice(double price) noexcept
	{
		app_.addDataItem(app_.dividingLine(), IsFormated::Disable);

		app_.addDataItem("Цена игры: " + std::to_string(price));

		app_.addDataItem(app_.dividingLine(), IsFormated::Disable);
	}


	void Solution::printOptimalStrategy(const std::vector<std::vector<int32_t>>& data
		, std::string&& player_name
		, std::string&& variable_name
		, MatrixAlignment ma
		, const PairCoords& coord) noexcept
	{
		assert(data.size() == 2 && "Incorrect data!");
		auto data_from_base{ getValueFromData(data, coord) };

		app_.addDataItem({ "Стратегия " + player_name + ": "
			+ std::to_string(data_from_base.first) + variable_name + "1 + "
			+ std::to_string(data_from_base.second) + variable_name + "2 = V"
			});
	}

	[[nodiscard]] std::pair<double, double> Solution::coefficientСalculation(const std::vector<std::vector<int32_t>>& data
		, PairCoords coof_left, PairCoords coof_right) const noexcept
	{

		assert(data.size() == 2 && "Incorrect data!");
		auto lhs{ getValueFromData(data, coof_left) };
		auto rhs{ getValueFromData(data, coof_right) };

		double a{ 1.0 * (lhs.first - rhs.first) };
		double b{ 1.0 * (lhs.second - rhs.second) };

		double tmp{ ((-1.0 * b) / a)+1 };
		tmp = 1 / tmp;
		return { 1 - tmp, tmp };
	}

	double Solution::generateRandom() const noexcept
	{
		return randFromRange<double>(0., 1.);
	}
}