﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(GravityOrbiter))]
[RequireComponent(typeof(CircleCollider2D))]
class TankPlayerController : MonoBehaviour 
{

	private CircleCollider2D Collider;
	private GravityOrbiter Orbiter;

	public float TreadSpeed;
	public float TreadGrip;
	public float JumpForce;
	public bool IsGrounded {get; private set;}
	private Vector2 GroundNormal;

	// Use this for initialization
	void Start () {
		Collider = GetComponent<CircleCollider2D>();
		Orbiter = GetComponent<GravityOrbiter>();
	}
	
	void OnCollisionEnter2D(Collision2D col)
	{
		IsGrounded = true;

		// Ground normal is average of all contact normals I guess
		GroundNormal *= 0.0f;
		foreach(var contact in col.contacts)
		{
			GroundNormal += contact.normal;
		}
		GroundNormal.Normalize();

	}

	// Update is called once per frame
	void Update () 
	{
		Debug.DrawRay(transform.position, GroundNormal, Color.red, 0.1f);
		Debug.DrawRay(transform.position, Orbiter.RelativeUp, Color.green, 0.1f);


		if(IsGrounded)
		{
			transform.up = GroundNormal;

			Vector2 normalForce;
			if(Input.GetButtonDown("Jump"))
			{
				normalForce = GroundNormal * JumpForce;
				IsGrounded = false;
			}
			else
			{
				normalForce = -GroundNormal * TreadGrip;
			}

			GetComponent<Rigidbody2D>().AddForce(normalForce, ForceMode2D.Force);

			// Apply relative horizontal movement
			if(Input.GetAxis("Horizontal") > 0.1f)
			{
				Vector3 moveForce = TreadSpeed * transform.right;
				GetComponent<Rigidbody2D>().AddForce(moveForce, ForceMode2D.Force);

			}
			else if(Input.GetAxis("Horizontal") < -0.1f)
			{
				Vector3 moveForce = -TreadSpeed * transform.right;
				GetComponent<Rigidbody2D>().AddForce(moveForce, ForceMode2D.Force);
			}
		}
		else
		{
			transform.up = Orbiter.RelativeUp;
		}
	}



}
