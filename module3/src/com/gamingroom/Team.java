package com.gamingroom;

import java.util.ArrayList;
import java.util.List;

/**
 * A simple class to hold information about a team
 * <p>
 * Notice the overloaded constructor that requires
 * an id and name to be passed when creating.
 * Also note that no mutators (setters) defined so
 * these values cannot be changed once a team is
 * created.
 * </p>
 * @author coce@snhu.edu
 *
 */
public class Team extends Entity {
	
	private List<Player> players = new ArrayList<Player>();
	
	private long nextPlayerId = 1;
	
	/*
	 * Constructor with an identifier and name
	 */
	public Team(long id, String name) {
		super(id, name);
	}

	/**
	 * @return the id
	 */
	public long getId() {
		return super.getId();
	}

	/**
	 * @return the name
	 */
	public String getName() {
		return super.getName();
	}
	
	public Player addPlayer(String name) {
		
		Player newPlayer;
		
		// If no teams exist, add the team
		if (players.isEmpty()) {
			newPlayer = new Player(nextPlayerId, name);
			players.add(newPlayer);
			nextPlayerId ++;
			return newPlayer;
		} else {
			// If teams do exist, make sure none have the name we are trying
			// to use, then add that team
			for (Player p: players) {			
				if ( p.getName().equals(name)) {
					// Team already exists in game. return and don't add team.
					return null;
				} else {
					// Do nothing
				}
			}
			// If we get through all the teams and the team doesn't already exist
			// add the team to teams.
			newPlayer = new Player(nextPlayerId + 1, name);
			players.add(newPlayer);
			nextPlayerId ++;
			return newPlayer;
		}
	}

	@Override
	public String toString() {
		return "Team [id=" + getId() + ", name=" + getName() + "]";
	}
}
