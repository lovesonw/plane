#include <Book/GameState.hpp>
#include <Book/MusicPlayer.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

GameState::GameState(StateStack& stack, Context context)
: State(stack, context)
, mWorld(*context.window, *context.fonts, *context.sounds)
, mPlayer(*context.player)
{
	mPlayer.setMissionStatus(Player::MissionRunning);
	// 播放主背景音乐
	context.music->play(Music::MissionTheme);
}

void GameState::draw()
{
	mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);
	if (!mWorld.hasAlivePlayer())
	{
		mPlayer.setMissionStatus(Player::MissionFailure);
		requestStackPush(States::GameOver);
	}
	else if (mWorld.hasPlayerReachedEnd())
	{
		mPlayer.setMissionStatus(Player::MissionSuccess);
		requestStackPush(States::GameOver);
	}
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleRealtimeInput(commands);
	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	// 游戏输入处理
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayer.handleEvent(event, commands);
	// Esc键按下，暂停游戏
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);
	return true;
}
