/*
*********************************************************************
* File          : SFMLSlot.cpp
* Project		: DragonBonesSFML
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
* License   	: MIT License
*********************************************************************
*/

#include "SFMLSlot.h"

#include <SFML\Graphics.hpp>

#include "SFMLMesh.h"
#include "SFMLArmatureDisplay.h"
#include "SFMLTextureAtlasData.h"
#include "SFMLTextureData.h"

DRAGONBONES_NAMESPACE_BEGIN

void SFMLSlot::_updateVisible()
{
	_renderDisplay->visible = _parent->getVisible();
}

void SFMLSlot::_updateBlendMode()
{
	if (_renderDisplay)
	{
		switch (_blendMode)
		{
			case BlendMode::Normal:
				_renderDisplay->blendMode = sf::BlendMode();
				break;

			case BlendMode::Add:
				_renderDisplay->blendMode = sf::BlendAdd;
				break;

			default:
				break;
		}
	}
	else if (_childArmature)
	{
		for (const auto slot : _childArmature->getSlots())
		{
			slot->_blendMode = _blendMode;
			slot->_updateBlendMode();
		}
	}
}

void SFMLSlot::_updateColor()
{
	if (_renderDisplay)
	{
		sf::Color helpColor;

		helpColor.a = _colorTransform.alphaMultiplier * 255.0f;
		helpColor.r = _colorTransform.redMultiplier * 255.0f;
		helpColor.g = _colorTransform.greenMultiplier * 255.0f;
		helpColor.b = _colorTransform.blueMultiplier * 255.0f;

		_renderDisplay->setColor(helpColor);
	}
}

void SFMLSlot::_initDisplay(void* value)
{
}

void SFMLSlot::_disposeDisplay(void* value)
{
}

void SFMLSlot::_onUpdateDisplay()
{
	_renderDisplay = std::unique_ptr<SFMLDisplay>(static_cast<SFMLDisplay*>(_display != nullptr ? _display : _rawDisplay));
}

void SFMLSlot::_addDisplay()
{
}

void SFMLSlot::_replaceDisplay(void* value, bool isArmatureDisplay)
{
}

void SFMLSlot::_removeDisplay()
{
}

void SFMLSlot::_updateZOrder()
{
}

void SFMLSlot::_updateFrame()
{
	const auto meshData = _display == _meshDisplay ? _meshData : nullptr;
	auto currentTextureData = static_cast<SFMLTextureData*>(_textureData);

	if (_displayIndex >= 0 && _display != nullptr && currentTextureData != nullptr)
	{
		if (currentTextureData->texture != nullptr)
		{
			if (meshData != nullptr) // Mesh
			{
				const auto data = meshData->parent->parent->parent;
				const auto intArray = data->intArray;
				const auto floatArray = data->floatArray;
				const unsigned vertexCount = intArray[meshData->offset + (unsigned)BinaryOffset::MeshVertexCount];
				const unsigned triangleCount = intArray[meshData->offset + (unsigned)BinaryOffset::MeshTriangleCount];
				int vertexOffset = intArray[meshData->offset + (unsigned)BinaryOffset::MeshFloatOffset];

				if (vertexOffset < 0)
				{
					vertexOffset += 65536;
				}

				const unsigned uvOffset = vertexOffset + vertexCount * 2;

				const auto& region = currentTextureData->region;

				std::vector<sf::Vertex> vertices(vertexCount);

				std::vector<std::vector<int>> verticesInTriagles;

				std::vector<uint16_t> vertexIndices(triangleCount * 3);

				for (std::size_t i = 0, l = vertexCount * 2; i < l; i += 2)
				{
					const auto iH = i / 2;

					const auto x = floatArray[vertexOffset + i];
					const auto y = floatArray[vertexOffset + i + 1];
					auto u = floatArray[uvOffset + i];
					auto v = floatArray[uvOffset + i + 1];

					sf::Vertex vertexData;
					vertexData.position = { x, y };

					if (currentTextureData->rotated)
					{
						vertexData.texCoords.x = (region.x + (1.0f - v) * region.width);
						vertexData.texCoords.y = (region.y + u * region.height);
					}
					else
					{
						vertexData.texCoords.x = (region.x + u * region.width);
						vertexData.texCoords.y = (region.y + v * region.height);
					}

					vertexData.color = sf::Color::White;
					
					vertices[iH] = vertexData;
				}

				for (std::size_t i = 0; i < triangleCount * 3; ++i)
				{
					vertexIndices.push_back(intArray[meshData->offset + (unsigned)BinaryOffset::MeshVertexIndices + i]);
				}

				std::vector<sf::Vertex> verticesDisplay;

				verticesInTriagles.resize(vertices.size());

				// sorting
				for (uint64_t i = 0; i < vertexIndices.size(); i++)
				{
					verticesInTriagles[vertexIndices[i]].push_back(i);
					verticesDisplay.push_back(vertices[vertexIndices[i]]);
				}

				_textureScale = 1.f;

				auto meshDisplay = new SFMLMesh();
				meshDisplay->texture = currentTextureData->texture;
				meshDisplay->verticesDisplay = std::move(verticesDisplay);
				meshDisplay->verticesInTriagles = std::move(verticesInTriagles);

				_renderDisplay->meshDisplay = std::unique_ptr<SFMLMesh>(meshDisplay);

			}
			else // Normal texture
			{
				const auto scale = currentTextureData->parent->scale * _armature->_armatureData->scale;
				const auto height = (currentTextureData->rotated ? currentTextureData->region.width : currentTextureData->region.height) * scale;
				_pivotY -= height;
				_textureScale = scale; 

				auto spriteDisplay = std::make_unique<sf::Sprite>();
				spriteDisplay->setTexture(*currentTextureData->texture);
				spriteDisplay->setTextureRect(currentTextureData->textureRect);
				spriteDisplay->setOrigin({ 0.f, spriteDisplay->getLocalBounds().height });
				_renderDisplay->spriteDisplay = std::move(spriteDisplay);
			}

			_visibleDirty = true;
			_blendModeDirty = true;
			_colorDirty = true;

			return;
		}
	}

	_renderDisplay->visible = false;
}

void SFMLSlot::_updateMesh()
{
	const auto hasFFD = !_ffdVertices.empty();
	const auto scale = _armature->_armatureData->scale;
	const auto textureData = static_cast<SFMLTextureData*>(_textureData);
	const auto meshData = _meshData;
	const auto weightData = meshData->weight;
	const auto meshDisplay = _renderDisplay->meshDisplay.get();

	if (!textureData || meshDisplay->texture != textureData->texture)
	{
		return;
	}

	if (weightData != nullptr)
	{
		const auto data = meshData->parent->parent->parent;
		const auto intArray = data->intArray;
		const auto floatArray = data->floatArray;
		const auto vertexCount = (std::size_t)intArray[meshData->offset + (unsigned)BinaryOffset::MeshVertexCount];
		int weightFloatOffset = intArray[weightData->offset + (unsigned)BinaryOffset::WeigthFloatOffset];

		if (weightFloatOffset < 0)
		{
			weightFloatOffset += 65536;
		}

		for (
			std::size_t i = 0, iD = 0, iB = weightData->offset + (unsigned)BinaryOffset::WeigthBoneIndices + weightData->bones.size(), iV = (std::size_t)weightFloatOffset, iF = 0;
			i < vertexCount;
			++i
			)
		{
			const auto boneCount = (std::size_t)intArray[iB++];
			auto xG = 0.0f, yG = 0.0f;
			for (std::size_t j = 0; j < boneCount; ++j)
			{
				const auto boneIndex = (unsigned)intArray[iB++];
				const auto bone = _meshBones[boneIndex];
				if (bone != nullptr)
				{
					const auto& matrix = bone->globalTransformMatrix;
					const auto weight = floatArray[iV++];
					auto xL = floatArray[iV++] * scale;
					auto yL = floatArray[iV++] * scale;

					if (hasFFD)
					{
						xL += _ffdVertices[iF++];
						yL += _ffdVertices[iF++];
					}

					xG += (matrix.a * xL + matrix.c * yL + matrix.tx) * weight;
					yG += (matrix.b * xL + matrix.d * yL + matrix.ty) * weight;
				}
			}

			auto& vertsDisplay = meshDisplay->verticesDisplay;

			for (auto vert : meshDisplay->verticesInTriagles[i])
			{
				auto& vertexPosition = vertsDisplay[vert].position;
				vertexPosition = { xG, yG };
			}
		}
	}
	else if (hasFFD)
	{
		const auto data = meshData->parent->parent->parent;
		const auto intArray = data->intArray;
		const auto floatArray = data->floatArray;
		const auto vertexCount = (std::size_t)intArray[meshData->offset + (unsigned)BinaryOffset::MeshVertexCount];
		int vertexOffset = (std::size_t)intArray[meshData->offset + (unsigned)BinaryOffset::MeshFloatOffset];

		if (vertexOffset < 0)
		{
			vertexOffset += 65536;
		}

		for (std::size_t i = 0, l = vertexCount * 2; i < l; i += 2)
		{
			const auto iH = i / 2;
			const auto xG = floatArray[vertexOffset + i] * scale + _ffdVertices[i];
			const auto yG = floatArray[vertexOffset + i + 1] * scale + _ffdVertices[i + 1];

			auto& vertsDisplay = meshDisplay->verticesDisplay;

			for (auto vert : meshDisplay->verticesInTriagles[iH])
			{
				auto& vertexPosition = vertsDisplay[vert].position;
				vertexPosition = { xG, yG };
			}
		}
	}
}

void SFMLSlot::_updateTransform(bool isSkinnedMesh)
{
	if (isSkinnedMesh)
	{
		_renderDisplay->setMatrix(Matrix(), sf::Vector2f(), _textureScale);
	}
	else
	{
		sf::Vector2f pos;

		if (_renderDisplay.get() == _rawDisplay || _renderDisplay.get() == _meshDisplay)
		{
			globalTransformMatrix.ty = globalTransformMatrix.ty;

			pos.x = globalTransformMatrix.tx - (globalTransformMatrix.a * _pivotX + globalTransformMatrix.c * _pivotY);
			pos.y = globalTransformMatrix.ty - (globalTransformMatrix.b * _pivotX + globalTransformMatrix.d * _pivotY);
		}
		else if (_childArmature)
		{
			globalTransformMatrix.ty = globalTransformMatrix.ty;

			pos.x = globalTransformMatrix.tx;
			pos.y = globalTransformMatrix.ty;
		}
		else
		{
			sf::Vector2f anchorPoint = { 1.f, 1.f };
			pos.x = globalTransformMatrix.tx - (globalTransformMatrix.a * anchorPoint.x - globalTransformMatrix.c * anchorPoint.y);
			pos.y = globalTransformMatrix.ty - (globalTransformMatrix.b * anchorPoint.x - globalTransformMatrix.d * anchorPoint.y);
		}

		_renderDisplay->setMatrix(globalTransformMatrix, pos, _textureScale);
	}
}

void SFMLSlot::_onClear()
{
	Slot::_onClear();

	_textureScale = 1.0f;

	if (_textureData)
	{
		delete _textureData;
		_textureData = nullptr;
	}
}

DRAGONBONES_NAMESPACE_END
