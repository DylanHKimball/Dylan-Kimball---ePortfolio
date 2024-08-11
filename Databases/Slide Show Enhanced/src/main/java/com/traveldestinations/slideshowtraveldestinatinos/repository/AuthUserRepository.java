package com.traveldestinations.slideshowtraveldestinatinos.repository;

import java.util.Optional;

import org.springframework.data.mongodb.repository.MongoRepository;

import com.traveldestinations.slideshowtraveldestinatinos.models.AuthUser;

// Class extends from the Mongodb repository which allows for all CRUD operations (Create, Read, Update, Delete) 
public interface AuthUserRepository extends MongoRepository<AuthUser, String>{
	
	Optional<AuthUser> findByUsername(String username); // Function to find a user in MongoDB by the username
}
