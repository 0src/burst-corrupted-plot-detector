/* Burst Corrupted Plot Detector - Scans Burst mining logs and reports
* possible corrupt plot files.
* Copyright(C) 2018 Guney Ozsan
*
* This program is free software : you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.If not, see <https://www.gnu.org/licenses/>.
*/

#include "plot_files.h"

bool
plot_files::contains(const std::string &plot_file_name) {
	if (plot_file_collection.count(plot_file_name) == 0)
		return false;
	else
		return true;
}

void
plot_files::add(const std::string &plot_file_name) {
	plot_file_collection[plot_file_name] = plot_file(plot_file_name);
}

plot_file
plot_files::get(const std::string &plot_file_name) {
	return plot_file_collection[plot_file_name];
}

std::map<std::string, plot_file>
plot_files::get_collection() {
	return plot_file_collection;
}

std::vector<plot_file>
plot_files::get_vector() {
	std::vector<plot_file> plot_files_vector(plot_file_collection.size());
	int i = 0;
	for (auto &it_pf : plot_file_collection) {
		it_pf.second.mining_stats.AddCorruptedCount(it_pf.second.found_deadlines.size());
		plot_files_vector[i] = it_pf.second;
		i++;
	}
	return plot_files_vector;
}

void
plot_files::add_found_deadline(const std::string &plot_file_name, const std::string &found_deadline) {
	plot_file_collection[plot_file_name].found_deadlines.push_back(found_deadline);
}

std::string
plot_files::find_plot_file_with_deadline(const std::string &confirmed_deadline) {
	for (auto &it_pf : plot_file_collection) {
		auto it_fd = it_pf.second.found_deadlines.begin();
		size_t j = 0;
		while (j < it_pf.second.found_deadlines.size()) {
			if (*it_fd == confirmed_deadline) {
				return it_pf.second.name;
			}
			else {
				it_fd++;
				j++;
			}
		}
	}
	return "";
}

void
plot_files::remove_deadline(const std::string &plot_file_name, const std::string &deadline) {
	auto it_fd = plot_file_collection[plot_file_name].found_deadlines.begin();
	size_t j = 0;
	while (j < plot_file_collection[plot_file_name].found_deadlines.size()) {
		if (*it_fd == deadline) {
			plot_file_collection[plot_file_name].found_deadlines.erase(it_fd);
			plot_file_collection[plot_file_name].mining_stats.AddHealthy();
			break;
		}
		else {
			it_fd++;
			j++;
		}
	}
}