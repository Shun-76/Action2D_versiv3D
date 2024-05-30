# include <Siv3D.hpp> // Siv3D v0.6.14
#include "Asset.h"

void InitAsset(void)
{
	asset_action_coin = U"action_coin";
	TextureAsset::Register(asset_action_coin, U"Action/Texture/action_coin.tga");

	asset_action_ninja = U"action_ninja";
	TextureAsset::Register(asset_action_coin, U"Action/Texture/action_ninja.tga");

	asset_arrow_up = U"arrow_up";
	TextureAsset::Register(asset_arrow_up, U"Action/Texture/arrow-up.tga");

	asset_axe = U"axe";
	TextureAsset::Register(asset_axe, U"Action/Texture/axe.tga");

	asset_backg = U"backg";
	TextureAsset::Register(asset_backg, U"Action/Texture/backg.tga");

	asset_bg = U"bg";
	TextureAsset::Register(asset_bg, U"Action/Texture/bg.tga");

	asset_block = U"block";
	TextureAsset::Register(asset_block, U"Action/Texture/block.tga");

	asset_bokan = U"bokan";
	TextureAsset::Register(asset_bokan, U"Action/Texture/bokan.tga");
}

void UninitAsset(void)
{

}
