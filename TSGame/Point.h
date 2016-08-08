#pragma once

struct Point {
	int x, y;

	Point() = default;
	Point(int x, int y) : x(x), y(y) {}
	Point(int size) : x(size), y(size) {}

	Point operator -() const {
		return {-x, -y};
	}

	Point operator +(const Point& p) const {
		return {x + p.x, y + p.y};
	}

	Point operator -(const Point& p) const {
		return {x - p.x, y - p.y};
	}

	Point operator *(const Point& p) const {
		return {x * p.x, y * p.y};
	}

	Point operator /(const Point& p) const {
		return {x / p.x, y / p.y};
	}

	Point operator *(const int n) const {
		return {x * n, y * n};
	}

	Point operator /(const int n) const {
		return {x / n, y / n};
	}

	Point& operator +=(const Point& p) {
		x += p.x;
		y += p.y;
		return *this;
	}

	Point& operator -=(const Point& p) {
		x -= p.x;
		y -= p.y;
		return *this;
	}

	bool operator ==(const Point& p) const {
		return x == p.x && y == p.y;
	}

	bool isZero() const {
		return x == 0 && y == 0;
	}
};

using Size = Point;
