#pragma once
#include <vector>
#include <utility>
#include "Vector2D.h"

class Figure;

class IField
{
public:
	IField() noexcept = default;
	IField(const IField&) = delete;
	virtual ~IField() noexcept = default;

	IField& operator=(const IField&) = delete;

	virtual bool TryPlaceFigure(Figure figure) noexcept = 0;
	virtual void Clear() noexcept = 0;

	virtual std::pair<bool, Vector2i> TryGetCellIndex(Vector2f worldPointInCell) const noexcept = 0;
	virtual const std::vector<Figure>& GetFigures() const noexcept = 0;
};
