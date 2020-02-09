package com.gamingroom;

public class Entity {

    private long id;
    private String name;

    private Entity(){
        
    }
    
    public Entity(long mId, String mName) {
    	id = mId;
    	name = mName;
    }
    
    public long getId() {
    	return id;
    }
    
    public String getName() {
    	return name;
    }
    
    public String toString() {
    	return "";
    }
}
