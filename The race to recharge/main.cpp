#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

struct Event {
  int x, y;
  bool is_add; // true = добавление, false = удаление
};

int main() {
  int n, Y, X;
  std::cin >> n >> Y >> X;

  std::vector<Event> events;
  for (int i = 0; i < n; ++i) {
    int x, y1, y2;
    std::cin >> x >> y1 >> y2;
    events.push_back({x, y1, true});    // Добавление заправки
    events.push_back({x, y2 + 1, false}); // Удаление заправки
  }

  // Сортируем события: сначала по y, затем удаления перед добавлением
  std::sort(events.begin(), events.end(), [](const Event& a, const Event& b) {
    if (a.y == b.y) return a.is_add < b.is_add;
    return a.y < b.y;
  });

  std::set<int> active;         // Активные координаты x
  std::multiset<int> gaps;      // Разрывы между координатами
  active.insert(0);             // Левая граница трассы
  active.insert(X);             // Правая граница трассы
  gaps.insert(X);               // Изначальный разрыв

  std::vector<int> result(Y + 1); // Ответы для всех трасс
  size_t event_idx = 0;

  for (int y = 0; y <= Y; ++y) {
    // Обрабатываем все события на текущей линии y
    while (event_idx < events.size() && events[event_idx].y == y) {
      const auto& event = events[event_idx];
      if (event.is_add) {
        // Добавляем заправку
        auto it = active.insert(event.x).first; // Вставка в set
        auto prev = std::prev(it);
        auto next = std::next(it);

        // Удаляем старый разрыв и добавляем два новых
        gaps.erase(gaps.find(*next - *prev));
        gaps.insert(*it - *prev);
        gaps.insert(*next - *it);
      } else {
        // Удаляем заправку
        auto it = active.find(event.x);
        auto prev = std::prev(it);
        auto next = std::next(it);

        // Удаляем два разрыва и добавляем один новый
        gaps.erase(gaps.find(*it - *prev));
        gaps.erase(gaps.find(*next - *it));
        gaps.insert(*next - *prev);

        active.erase(it); // Удаляем точку из set
      }
      ++event_idx;
    }

    // Максимальный разрыв находится в конце multiset
    result[y] = *gaps.rbegin();
  }

  // Вывод: Один результат на строку
  for (int i = 0; i <= Y; ++i) {
    std::cout << result[i] << '\n';
  }

  return 0;
}
