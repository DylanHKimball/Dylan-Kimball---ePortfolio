package com.traveldestinations.slideshowtraveldestinatinos.models;

import org.springframework.data.mongodb.core.mapping.Document;

// Destination model, sets up the attributes and layout for the destination
// objects which contain an id, image, title, and description.
// There is a constructor, setter, getter, and toString method
@Document(collection = "SlideShowCollection")
public class Destination {
	private String id;
	private String destID;
	private String image;
	private String title;
	private String description;
	private String imageFileLoc = "/images/";
	
	// CONSTRUCTOR
	public Destination(final String id,
					   final String image,
					   final String title,
					   final String description) {
		this.id = id;
		this.destID = id;
		this.image = image;
		this.title = title;
		this.description = description;
	}
	
	// GETTER FUNCTIONS
	public String getID() {
		return id;
	}
	
	public String getImage() {
		return image;
	}
	
	public String getTitle() {
		return title;
	}
	
	public String getDescription() {
		return description;
	}
	
	public String getDestID() {
		return destID;
	}
	
	
	// SETTER FUNCTIONS
	public void setID(String id) {
		this.id = id;
	}
	
	public void setImage(String image) {
		if(image.contains("/images/")) {
			this.image = image;
		}
		else {
			this.image = imageFileLoc + image;
		}
	}
	
	public void setTitle(String title) {
		this.title = title;
	}
	
	public void setDescription(String description) {
		this.description = description;
	}
	
	public void setDestID(String destID) {
		this.destID = destID;
	}
	
	
	// TO STRING FOR DEBUGGING NEEDS
	@Override
	public String toString() {
		return "Destination{" +
				"_id='" + id + '\'' +
				"image='" + image + '\'' +
				", title='" + title + '\'' +
				", description='" + description + '}';
	}
}
