/*
*    ||  -- Travel Destinations Website -- ||
*    |  Programmers Name: Dylan Kimball
*    |  Date: 8/7/2024
*    |  Version: 1.0
* 
*	 This code was created to build a website that displays travel destinations that are stored
*    in a NoSQL MongoDB database. Users are able to scroll through the page to see all destinations.
*    An admin can login using authorized credentials which will allow them to either add, delete,
*    or modify destination records in the database. The original program was a simple java file that
*    displayed this information on a separate window of the users computer, however it has been
*    completely rewritten to be a website.
* 
* 	 With this application, the admin pages is something that only admins should be able to access.
* 	 with that in mind, i've excluded the feature where a user can register themselves as the logins
*    are something that the company admin will be setting up elsewhere. Below are the test login
*    credentials that are currently in the database and allow for access to the admin page. All
*    passwords in the database are encrypted using BCrypt to ensure password security.
* 
* 	 TEST LOGIN TO ACCESS ADMIN PAGE
* 	 Username: TestAdmin
* 	 Password: TestPassword
* 
*	 DEPENDANCIES:
*		- Utilizes Spring boot and Spring Security to add web functionality and security
*		- Utilizes Spring MongoDB support to connect to database stored in the MongoDB Atlas cloud
*   	- Utilizes Maven to run the program
*		- Utilizes Thymeleaf to add more functionality to HTML views
*		- Utilizes a CSS Flexbox model to create a dynamic web page
*/

package com.traveldestinations.slideshowtraveldestinatinos;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

// Function that runs the application and boots up the server
@SpringBootApplication
public class Application {

	public static void main(String[] args) {
		SpringApplication.run(Application.class, args);
	}

}
