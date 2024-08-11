package com.traveldestinations.slideshowtraveldestinatinos.controllers;

import com.traveldestinations.slideshowtraveldestinatinos.models.Destination;
import com.traveldestinations.slideshowtraveldestinatinos.repository.DestinationRepository;

import java.util.ArrayList;
import java.util.List;
import org.springframework.web.servlet.ModelAndView;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

// This class is the controller that handles all the requests for the destination model
@Controller
public class DestinationController 
{
	// Sets up repositories to be used by mapping functions
	@Autowired
	DestinationRepository destinationRepo;
	

	// This method creates a model and view using the Destination model and printDestinations view
	// A find all query is completed on the destination repository and returns all records to a list
	// This list is then passed to the model and view object and then returned to display the page
	// with all destinations in the database.
	@GetMapping("/")
	public ModelAndView showDestinations() {	
		ModelAndView modelAndView = new ModelAndView("printDestinations"); // defines the Model And view object

		List<Destination> destinations = new ArrayList<Destination>(); // Create list of destination objects
		destinations = destinationRepo.findAll(); // Use the final all query on the database and add them to the list

		modelAndView.addObject("destinations", destinations); // Adds the list to the Model And View object
		
		return modelAndView;
	}
	
	
	// This method is the same as the above process, however it redirects to the admin page
	@RequestMapping(value="/admin", method = RequestMethod.GET)
	public ModelAndView showDestinationsAdmin() {	
		ModelAndView modelAndView = new ModelAndView("printDestinationsAdmin"); // defines the Model And view object

		List<Destination> destinations = new ArrayList<Destination>(); // Create list of destination objects
		destinations = destinationRepo.findAll(); // Use the final all query on the database and add them to the list

		modelAndView.addObject("destinations", destinations); // Adds the list to the Model And View object
		
		return modelAndView;
	}

	
	// This method displays the login HTML page when called
	@GetMapping("/login")
	public ModelAndView showLogin() {	
		ModelAndView modelAndView = new ModelAndView("login"); // defines the Model And view object
		
		return modelAndView;
	}
	
	
	// This method displays the add destination HTML page when called
	@GetMapping("/addDestination")
	public ModelAndView showAddDestionation() {	
		ModelAndView modelAndView = new ModelAndView("addDestination"); // defines the Model And view object
		
		return modelAndView;
	}
	
	
	// This method displays the add destination HTML page when called
	@GetMapping("/updateDestination")
	public ModelAndView showUpdateDestination() {	
		ModelAndView modelAndView = new ModelAndView("updateDestination"); // defines the Model And view object
		
		return modelAndView;
	}
	
	
	// This method takes information for a new destination and adds it to the database through the
	// destination repository.	
	@PostMapping("/add")
	public String addDestination(@ModelAttribute final Destination destination) {
		destinationRepo.save(destination); // Adds a destination object to the mongoDB database
				
		return "redirect:/admin"; // Returns to the admin page after adding record
	}
	
	
	// This method pulls up the page to have a admin enter information to modify an existing destination
	// and update the database through the destination repository. 
	//The id is passed from the HTML admin page when the update button is pressed on one of the destination cards.
	//The destination being modified has its information displayed as the default in the text input fields.
	@GetMapping("/showUpdate/{id}")
	public ModelAndView ShowUpdateDestination(@PathVariable(name="id") final String id) {
		ModelAndView modelAndView = new ModelAndView("updateDestination"); // defines the Model And view object

		modelAndView.addObject("destination", destinationRepo.findById(id)); // Adds the single destination to the Model And View object
			
		return modelAndView;
	}
	
	
	// This method pulls up the page to have a admin enter information to modify an existing destination
	// and update the database through the destination repository.
	@PostMapping("/update")
	public String updateDestination(@ModelAttribute final Destination destination) {
		destinationRepo.save(destination); // Saves the updated record over the record in the database with the matching id
		
		return "redirect:/admin"; // Returns to the admin page after adding record
	}
	
	
	// This method deletes the record of a specific destination by using its id to remove it form the 
	// database through the destination repository. The id is passed from the HTML admin page when the
	// delete button is pressed on one of the destination cards.
	@GetMapping("/delete/{id}")
	public String deleteDestination(@PathVariable(name="id") final String id) {
		destinationRepo.deleteById(id); // Deletes a destination object from the mongoDB database based on id passed through
		return "redirect:/admin"; // Returns to the admin page after adding record
	}

}
