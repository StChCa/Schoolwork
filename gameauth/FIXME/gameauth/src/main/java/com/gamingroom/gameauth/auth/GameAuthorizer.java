package com.gamingroom.gameauth.auth;

import io.dropwizard.auth.Authorizer;

public class GameAuthorizer implements Authorizer<GameUser> 
{
    @Override
    public boolean authorize(GameUser user, String role) {
    	
        // If user has a role, and user roles contains the param role, return true.
    	return user.getRoles() != null && user.getRoles().contains(role);
    	
    }
}