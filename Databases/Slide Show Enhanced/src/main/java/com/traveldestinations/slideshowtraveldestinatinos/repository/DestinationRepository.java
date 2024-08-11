package com.traveldestinations.slideshowtraveldestinatinos.repository;

import org.springframework.data.mongodb.repository.MongoRepository;

import com.traveldestinations.slideshowtraveldestinatinos.models.Destination;

// Class extends from the Mongodb repository which allows for all CRUD operations (Create, Read, Update, Delete) 
public interface DestinationRepository extends MongoRepository<Destination, String>{

}
