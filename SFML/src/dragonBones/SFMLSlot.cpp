/** @file SFMLSlot.cpp
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 ** @author Patryk (PsychoX) Ludwikowski <psychoxivi@gmail.com>
 ** @license MIT License
 **/

#include "SFMLSlot.h"

#include <SFML/Graphics.hpp>

#include "SFMLArmatureProxy.h"
#include "SFMLDisplay.h"
#include "SFMLTextureAtlasData.h"
#include "SFMLTextureData.h"

DRAGONBONES_NAMESPACE_BEGIN

void SFMLSlot::_updateVisible()
{
	_renderDisplay->setVisible(_parent->getVisible());
}

void SFMLSlot::_updateBlendMode()
{
	if (_renderDisplay)
	{
		auto display = static_cast<SFMLDisplay*>(_renderDisplay);

		switch (_blendMode)
		{
			case BlendMode::Normal:
				display->blendMode = sf::BlendMode();
				break;
			case BlendMode::Add:
				display->blendMode = sf::BlendAdd;
				break;
			case BlendMode::Multiply:
				display->blendMode = sf::BlendMultiply;
				break;
			default:
				display->blendMode = sf::BlendMode();
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
		sf::Color color;

		color.a = static_cast<uint8_t>(_colorTransform.alphaMultiplier * 255.f);
		color.r = static_cast<uint8_t>(_colorTransform.redMultiplier * 255.f);
		color.g = static_cast<uint8_t>(_colorTransform.greenMultiplier * 255.f);
		color.b = static_cast<uint8_t>(_colorTransform.blueMultiplier * 255.f);

		_renderDisplay->setColor(color);
	}
}

void SFMLSlot::_initDisplay(void* value, bool isRetain)
{
}

void SFMLSlot::_disposeDisplay(void* value, bool isRelease)
{
	if (!isRelease && value)
	{
		delete static_cast<SFMLNode*>(value);
	}
}

void SFMLSlot::_onUpdateDisplay()
{
	_renderDisplay = static_cast<SFMLNode*>(_display != nullptr ? _display : _rawDisplay);
	_renderDisplay->setZOffset(_slotData->zOrder);
}

void SFMLSlot::_addDisplay()
{
	auto arm = static_cast<SFMLArmatureProxy*>(_armature->getDisplay());
	arm->addNode(_renderDisplay);
}

void SFMLSlot::_replaceDisplay(void* value, bool isArmatureDisplay)
{
	auto prevDisplay = static_cast<SFMLNode*>(value);

	auto arm = static_cast<SFMLArmatureProxy*>(_armature->getDisplay());

	_renderDisplay->setZOffset(prevDisplay->getZOffset());

	arm->removeNode(prevDisplay);
	arm->addNode(_renderDisplay);

	arm->sortNodes();

	_textureScale = 1.f;

}

void SFMLSlot::_removeDisplay()
{
	auto arm = static_cast<SFMLArmatureProxy*>(_armature->getDisplay());
	arm->removeNode(static_cast<SFMLNode*>(_renderDisplay));
}

void SFMLSlot::_updateZOrder()
{
	_renderDisplay->setZOffset(_slotData->zOrder);
	auto arm = static_cast<SFMLArmatureProxy*>(_armature->getDisplay());
	arm->sortNodes();
}

void SFMLSlot::_updateFrame()
{
	const auto currentVerticesData = (_deformVertices != nullptr && _display == _meshDisplay) ? _deformVertices->verticesData : nullptr;
	auto currentTextureData = static_cast<SFMLTextureData*>(_textureData);

	auto display = static_cast<SFMLDisplay*>(_renderDisplay);

	if (_displayIndex >= 0 && _display != nullptr && currentTextureData != nullptr)
	{
		if (currentTextureData->texture != nullptr)
		{
			if (currentVerticesData != nullptr) // Mesh
			{
				const auto data = currentVerticesData->data;
				const auto intArray = data->intArray;
				const auto floatArray = data->floatArray;
				const unsigned vertexCount = intArray[currentVerticesData->offset + (unsigned)BinaryOffset::MeshVertexCount];
				const unsigned triangleCount = intArray[currentVerticesData->offset + (unsigned)BinaryOffset::MeshTriangleCount];
				int vertexOffset = intArray[currentVerticesData->offset + (unsigned)BinaryOffset::MeshFloatOffset];

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
					vertexIndices.push_back(intArray[currentVerticesData->offset + (unsigned)BinaryOffset::MeshVertexIndices + i]);
				}

				std::vector<sf::Vertex> verticesDisplay;

				verticesInTriagles.resize(vertices.size());

				// sorting
				for (unsigned int i = 0; i < vertexIndices.size(); i++)
				{
					verticesInTriagles[vertexIndices[i]].push_back(i);
					verticesDisplay.push_back(vertices[vertexIndices[i]]);
				}

				_textureScale = 1.f;

				display->texture = currentTextureData->texture;
				display->verticesDisplay = std::move(verticesDisplay);
				display->verticesInTriagles = std::move(verticesInTriagles);
				display->primitiveType = sf::PrimitiveType::Triangles;

				const auto isSkinned = currentVerticesData->weight != nullptr;
				if (isSkinned)
				{
					_identityTransform();
				}
			}
			else // Normal texture
			{
				const auto scale = currentTextureData->parent->scale * _armature->_armatureData->scale;
				const auto height = (currentTextureData->rotated ? currentTextureData->region.width : currentTextureData->region.height) * scale;
				_textureScale = scale; 

				auto texRect =currentTextureData->region;

				display->texture = currentTextureData->texture;

				display->verticesDisplay.resize(4);
				display->verticesDisplay[0].texCoords = sf::Vector2f(texRect.x, 					texRect.y);
				display->verticesDisplay[1].texCoords = sf::Vector2f(texRect.x, 					texRect.y + texRect.height);
				display->verticesDisplay[2].texCoords = sf::Vector2f(texRect.x + texRect.width, 	texRect.y);
				display->verticesDisplay[3].texCoords = sf::Vector2f(texRect.x + texRect.width, 	texRect.y + texRect.height);


				float boundsWidth = static_cast<float>(std::abs(texRect.width));
				float boundsheight = static_cast<float>(std::abs(texRect.height));

				display->verticesDisplay[0].position = sf::Vector2f(0.f, 0.f);
				display->verticesDisplay[1].position = sf::Vector2f(0.f, boundsheight);
				display->verticesDisplay[2].position = sf::Vector2f(boundsWidth, 0.f);
				display->verticesDisplay[3].position = sf::Vector2f(boundsWidth, boundsheight);

				display->setColor(sf::Color::White);
			}

			_visibleDirty = true;
			_blendModeDirty = true;
			_colorDirty = true;

			return;
		}
	}

	_renderDisplay->setVisible(false);
}

void SFMLSlot::_updateMesh()
{
	const auto scale = _armature->_armatureData->scale;
	const auto& deformVertices = _deformVertices->vertices;
	const auto& bones = _deformVertices->bones;
	const auto verticesData = _deformVertices->verticesData;
	const auto weightData = verticesData->weight;

	const auto hasFFD = !deformVertices.empty();
	const auto meshDisplay = static_cast<SFMLDisplay*>(_renderDisplay);


	if (weightData != nullptr)
	{
		const auto data = verticesData->data;
		const auto intArray = data->intArray;
		const auto floatArray = data->floatArray;
		const auto vertexCount = (std::size_t)intArray[verticesData->offset + (unsigned)BinaryOffset::MeshVertexCount];
		int weightFloatOffset = intArray[weightData->offset + (unsigned)BinaryOffset::WeigthFloatOffset];

		if (weightFloatOffset < 0)
		{
			weightFloatOffset += 65536;
		}

		for (
			std::size_t i = 0, iD = 0, iB = weightData->offset + (unsigned)BinaryOffset::WeigthBoneIndices + bones.size(), iV = (std::size_t)weightFloatOffset, iF = 0;
			i < vertexCount;
			++i
			)
		{
			const auto boneCount = (std::size_t)intArray[iB++];
			auto xG = 0.0f, yG = 0.0f;
			for (std::size_t j = 0; j < boneCount; ++j)
			{
				const auto boneIndex = (unsigned)intArray[iB++];
				const auto bone = bones[boneIndex];
				if (bone != nullptr)
				{
					const auto& matrix = bone->globalTransformMatrix;
					const auto weight = floatArray[iV++];
					auto xL = floatArray[iV++] * scale;
					auto yL = floatArray[iV++] * scale;

					if (hasFFD)
					{
						xL += deformVertices[iF++];
						yL += deformVertices[iF++];
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
		const auto data = verticesData->data;
		const auto intArray = data->intArray;
		const auto floatArray = data->floatArray;
		const auto vertexCount = (std::size_t)intArray[verticesData->offset + (unsigned)BinaryOffset::MeshVertexCount];
		int vertexOffset = (std::size_t)intArray[verticesData->offset + (unsigned)BinaryOffset::MeshFloatOffset];

		if (vertexOffset < 0)
		{
			vertexOffset += 65536;
		}

		for (std::size_t i = 0, l = vertexCount * 2; i < l; i += 2)
		{
			const auto iH = i / 2;
			const auto xG = floatArray[vertexOffset + i] * scale + deformVertices[i];
			const auto yG = floatArray[vertexOffset + i + 1] * scale + deformVertices[i + 1];

			auto& vertsDisplay = meshDisplay->verticesDisplay;

			for (auto vert : meshDisplay->verticesInTriagles[iH])
			{
				auto& vertexPosition = vertsDisplay[vert].position;
				vertexPosition = { xG, yG };
			}
		}
	}
}

void SFMLSlot::_identityTransform()
{
	_renderDisplay->setMatrix(Matrix(), sf::Vector2f(), _textureScale, _textureScale);
}

void SFMLSlot::_updateTransform()
{
	sf::Vector2f pos(
		globalTransformMatrix.tx,
		globalTransformMatrix.ty
	);

	if (_renderDisplay == _rawDisplay || _renderDisplay == _meshDisplay)
	{
		pos.x -= globalTransformMatrix.a * _pivotX + globalTransformMatrix.c * _pivotY;
		pos.y -= globalTransformMatrix.b * _pivotX + globalTransformMatrix.d * _pivotY;
	}
	else
	{
		pos.x -= globalTransformMatrix.a - globalTransformMatrix.c;
		pos.y -= globalTransformMatrix.b - globalTransformMatrix.d;
	}

	_renderDisplay->setMatrix(globalTransformMatrix, pos, _textureScale, _textureScale);
}

void SFMLSlot::_onClear()
{
	Slot::_onClear();

	_textureScale = 1.0f;
	_renderDisplay = nullptr;
}

DRAGONBONES_NAMESPACE_END
