package com.traveldestinations.slideshowtraveldestinatinos.services;

import org.springframework.stereotype.Service;

import com.traveldestinations.slideshowtraveldestinatinos.models.AuthUser;
import com.traveldestinations.slideshowtraveldestinatinos.repository.AuthUserRepository;

import java.util.Arrays;
import java.util.List;
import java.util.Optional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.userdetails.User;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;


// AuthUserService class uses the Spring Security UserDetailService class to load user data.
@Service
public class AuthUserService implements UserDetailsService{

	// Sets up repositories to be used by mapping functions
	@Autowired
	AuthUserRepository userRepo;
	
	// Function used to locate a user from the database based on the username passed through
	@Override
	public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
		Optional<AuthUser> authUser = userRepo.findByUsername(username); // Calls the user repository to find the record with the matching username
		
		// If no authorized user was found then the function will return throw an exception
		if (authUser == null) { 
			throw new UsernameNotFoundException("User Not Found"); 
		}
		// If an authorized user is found, then the function returns a new spring security user with the name and password found
		else {
	        List<SimpleGrantedAuthority> authorities = Arrays.asList(new SimpleGrantedAuthority( "ADMIN" )); // Sets user role to admins as only admins can login	
	        
			return new User(authUser.get().getUsername(), authUser.get().getPassword(), authorities);
		}
	}

}
