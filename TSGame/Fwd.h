#pragma once
#include <memory>
#include "Actor.h"

class GraphicDevice;
class TextureManager;
class SoundManager;
class InputManager;
class FPSControler;
class Random;
using GraphicDevicePtr = std::shared_ptr<GraphicDevice>;
using TextureMgrPtr = std::shared_ptr<TextureManager>;
using SoundMgrPtr = std::shared_ptr<SoundManager>;
using InputMgrPtr = std::shared_ptr<InputManager>;
using FPSControlerPtr = std::shared_ptr<FPSControler>;
using RandomPtr = std::shared_ptr<Random>;

class Player;
class Flail;
class Enemy;
class Bullet;
class Effect;
class Font;
using PlayerPtr = std::shared_ptr<Player>;
using FlailPtr = std::shared_ptr<Flail>;
using EnemyMgrPtr = std::shared_ptr<ActorManager<Enemy>>;
using BulletMgrPtr = std::shared_ptr<ActorManager<Bullet>>;
using EffectMgrPtr = std::shared_ptr<ActorManager<Effect>>;
using FontPtr = std::shared_ptr<Font>;
