package com.traveldestinations.slideshowtraveldestinatinos.configs;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.config.Customizer;
import org.springframework.security.config.annotation.authentication.builders.AuthenticationManagerBuilder;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.web.SecurityFilterChain;
import org.springframework.security.web.util.matcher.AntPathRequestMatcher;

import com.traveldestinations.slideshowtraveldestinatinos.services.AuthUserService;


// Security configuration class that replaces the default login page provided with spring security with a custom page
// The server allows all requests to the main page to be allowed while all other pages require a login to access
@Configuration
@EnableWebSecurity
public class SecurityConfig {
		
	
	@Autowired
	AuthUserService authUserService;
	
	// Function that sets the security configuration to use the authUserService class
	public SecurityConfig(AuthUserService authUserService) {
        this.authUserService = authUserService;
    }

	
	// Function that sets the authentication manager to authenticate users using the authUserService class and decrypting the password
    @Bean
    public AuthenticationManager customAuthenticationManager(HttpSecurity http) throws Exception {
        AuthenticationManagerBuilder authenticationManagerBuilder = http.getSharedObject(AuthenticationManagerBuilder.class);
        authenticationManagerBuilder.userDetailsService(authUserService)
            .passwordEncoder(bCryptPasswordEncoder());
        return authenticationManagerBuilder.build();
    }

    
    // Function that utilizes BCrypt to encode and decode passwords
    @Bean
    public BCryptPasswordEncoder bCryptPasswordEncoder() {
        return new BCryptPasswordEncoder();
    }
	
    
    // Sets up the default endpoints to be handled by spring security
    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
    	http
		.formLogin(form -> form
			.loginPage("/login") // Replaces the default login with the custom login page created
			.permitAll()
			.defaultSuccessUrl("/admin")
		);
    	
        return http.authorizeHttpRequests(request -> request.requestMatchers(new AntPathRequestMatcher("/"))
        			.permitAll()) // Permits all requests to the main home page (does not need a login)
        		.authorizeHttpRequests((auth) -> auth.anyRequest()
        					.authenticated()) // Make it so all other pages require a login and will be directed to the login page
        		.httpBasic(Customizer.withDefaults())
        		.build();
    }
}