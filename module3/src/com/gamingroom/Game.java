package com.gamingroom;

import java.util.ArrayList;
import java.util.List;

/**
 * A simple class to hold information about a game
 * 
 * <p>
 * Notice the overloaded constructor that requires
 * an id and name to be passed when creating.
 * Also note that no mutators (setters) defined so
 * these values cannot be changed once a game is
 * created.
 * </p>
 * 
 * @author coce@snhu.edu
 *
 */

// Add that this class extends Entity.
public class Game extends Entity {
	
	private List<Team> teams = new ArrayList<Team>();
	
	private long nextTeamId = 1;
	
	/**
	 * Constructor with an identifier and name
	 */
	public Game(long id, String name) {
		// Since our id and name attributes are inhereted from Entity, we no longer
		// need to set the Game level attributes.
		super(id, name);
	}

	/**
	 * @return the id
	 */
	public long getId() {
		// Updated to get id from parent Entity class
		return super.getId();
	}

	/**
	 * @return the name
	 */
	public String getName() {
		// Updated to get name from super
		return super.getName();
	}
	
	/*
	 * addTeam
	 * Adds a team to this game
	 * @return null
	 */
	public Team addTeam(String name) {
		
		Team newTeam;
		
		// If no teams exist, add the team
		if (teams.isEmpty()) {
			newTeam = new Team(nextTeamId, name);
			teams.add(newTeam);
			nextTeamId ++;
			return newTeam;
		} else {
			// If teams do exist, make sure none have the name we are trying
			// to use, then add that team
			for (Team t: teams) {				
				if ( t.getName().equals(name)) {
					// Team already exists in game. return and don't add team.
					return null;
				} else {
					// Do nothing
				}
			}
			// If we get through all the teams and the team doesn't already exist
			// add the team to teams.
			newTeam = new Team(nextTeamId + 1, name);
			teams.add(newTeam);
			nextTeamId ++;
			return newTeam;
		}
	}

	@Override
	public String toString() {
		// Updated to get our id and name from super.
		return "Game [id=" + getId() + ", name=" + getName() + "]";
	}

}
