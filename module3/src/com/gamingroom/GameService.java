package com.gamingroom;

import java.util.ArrayList;
import java.util.List;

/**
 * A singleton service for the game engine
 * 
 * @author coce@snhu.edu
 */
public class GameService {

	/**
	 * A list of the active games
	 */
	private static List<Game> games = new ArrayList<Game>();

	/*
	 * Holds the next game identifier
	 */
	private static long nextGameId = 1;

	/*
	 * Create a local instance of this class. Since our constructor is private,
	 * we know that we will only ever have this one instance of this object
	 * making it a singleton.
	 */
	private static GameService instance = new GameService();

	/*
	 * A private constructor. It is normal for a singleton to have a private
	 * construtor so that we don't instantiate any additional instances
	 * outside of this class.
	 */
	private void GameService(){
	}

	/*
	 * A public accessor for our instance. This allows outside classes to
	 * access objects in this singleton class.
	 */
	public static GameService getInstance(){
		return instance;
	}


	/**
	 * Construct a new game instance
	 *
	 * @param name the unique name of the game
	 * @return the game instance (new or existing)
	 */
	public Game addGame(String name) {

		// a local game instance
		Game game = null;

		// if Game with name already exists, simply return the existing instance
		// without adding anything to our games datastructure.
		for (Game currentGame : games) {
			if (currentGame.getName().equals(name)) {
				return currentGame;
			}
		}

		// if not found, make a new game instance and add to list of games
		if (game == null) {
			game = new Game(nextGameId++, name);
			games.add(game);
		}

		// return the new/existing game instance to the caller
		return game;
	}

	/**
	 * Returns the game instance at the specified index.
	 * <p>
	 * Scope is package/local for testing purposes.
	 * </p>
	 * @param index index position in the list to return
	 * @return requested game instance
	 */
	Game getGame(int index) {
		return games.get(index);
	}
	
	/**
	 * Returns the game instance with the specified id.
	 * 
	 * @param id unique identifier of game to search for
	 * @return requested game instance
	 */
	public Game getGame(long id) {

		// a local game instance
		Game game = null;

		
		// Parse the current list of games. If one already exists
		// with this id, return that Game instance.
		for (Game currentGame : games) {
			if (currentGame.getId() == id){
				game = currentGame;
			}
		}

		return game;
	}

	/**
	 * Returns the game instance with the specified name.
	 * 
	 * @param name unique name of game to search for
	 * @return requested game instance
	 */
	public Game getGame(String name) {

		// a local game instance
		Game game = null;

		// Parse the current list of games. If one already exists
		// with this name, return that Game instance.
		for (Game currentGame : games) {
			if (currentGame.getName().equals(name)){
				game = currentGame;
			}
		}

		return game;
	}

	/**
	 * Returns the number of games currently active
	 * 
	 * @return the number of games currently active
	 */
	public int getGameCount() {
		return games.size();
	}
}
