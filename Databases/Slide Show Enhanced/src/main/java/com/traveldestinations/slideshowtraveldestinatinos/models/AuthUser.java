package com.traveldestinations.slideshowtraveldestinatinos.models;

import org.springframework.data.mongodb.core.mapping.Document;

//Destination model, sets up the attributes and layout for the admin users
//objects which contain an id, username, and password.
//There is a constructor, setter, getter, and toString method
@Document(collection = "AuthenticatedUsers")
public class AuthUser {
	private String id;
	private String username;
	private String password;
	
	// CONSTRUCTOR
	public AuthUser(final String id,
					final String username,
					final String password) {
		this.id = id;
		this.username = username;
		this.password = password;
	}
	
	// GETTER FUNCTIONS
	public String getID() {
		return id;
	}
	
	public String getUsername() {
		return username;
	}
	
	public String getPassword() {
		return password;
	}
	
	
	// SETTER FUNCTIONS
	public void setID(String id) {
		this.id = id;
	}
	
	public void setUsernmae(String username) {
		this.username = username;
	}
	
	public void setPassword(String password) {
		this.password = password;
	}
	
	
	// TO STRING FOR DEBUGGING NEEDS
	@Override
	public String toString() {
		return "User{" +
				"_id='" + id + '\'' +
				"username='" + username + '\'' +
				", password='" + password + '}';
	}
}
