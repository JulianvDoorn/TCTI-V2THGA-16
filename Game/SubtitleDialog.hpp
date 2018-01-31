#pragma once

#include <SFML/Graphics.hpp>
#include <initializer_list>
#include <vector>

#include "GameObject.hpp"
#include "SubtitleItem.hpp"

class SubtitleDialog : public sf::Drawable, public GameObject, public std::vector<SubtitleItem> {
protected:
	SubtitleDialog(std::initializer_list<SubtitleItem>& lst) : std::vector<SubtitleItem>(lst) { }
};

class EventedSubtitleDialog : public SubtitleDialog {
	const SubtitleItem* getItemFromSubtitles() const {
		if (subtitleIndex < size()) {
			return &at(subtitleIndex);
		}

		return nullptr;
	}

	uint32_t subtitleIndex;
	bool eventFlag;

	EventConnection eventConn;

public:
	EventedSubtitleDialog(std::initializer_list<SubtitleItem> lst) : SubtitleDialog(lst) { }

	template<class CONDITION, class... Args>
	void waitForEvent(EventSource<Args...>& eventSource, CONDITION condition) {
		eventConn = eventSource.connect([this, &condition](Args... args) {
			if (condition(args...) == true) {
				eventFlag = true;
			}
		});
	}

	void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const override {
		const SubtitleItem* item = getItemFromSubtitles();

		if (item != nullptr) {
			std::cout << item->getText() << std::endl;
		}
	}

	void update(const float elapsedTime) override {
		if (eventFlag) {
			if (subtitleIndex < size()) { // prevent overflowing
				subtitleIndex++;
			}

			eventFlag = false;
		}
	}
};

class TimedSubtitleDialog : public SubtitleDialog {
	const SubtitleItem* getItemFromSubtitles() const {
		for (const SubtitleItem& item : *this) {
			if (time >= item.getTimeStart() && time < item.getTimeEnd()) {
				return &item;
			}
		}
		
		return nullptr;
	}

	float time;

public:
	TimedSubtitleDialog(std::initializer_list<SubtitleItem> lst) : SubtitleDialog(lst) { }

	void draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const override {
		const SubtitleItem* item = getItemFromSubtitles();

		if (item != nullptr) {
			std::cout << item->getText() << std::endl;
		}
	}

	void update(const float elapsedTime) override {
		time += elapsedTime;
	}
};