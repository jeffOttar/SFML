/**
* @file
* @author Jeff Ottar-
* @version 1.0
*
*
* @section DESCRIPTION
* 
*
*
* @section LICENSE
*
*
* Copyright 2017
* Permission to use, copy, modify, and/or distribute this software for
* any purpose with or without fee is hereby granted, provided that the
* above copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*
* @section Academic Integrity
* I certify that this work is solely my own and complies with
* NBCC Academic Integrity Policy (policy 1111)
*/
#pragma once

namespace Category {
	enum Type
	{
		None = 0,
		Scene = 1 << 0,//the << is a left bitshift --> this is 1 with 0 bitshift
		PlayerAircraft = 1 << 1,
		AlliedAircraft = 1 << 2,//this is with a 2 bitshift so 101 --> 10100 (the bit shifts to the left)
		EnemyAircraft = 1 << 3,//if the number goes past the limit 64bits then it drops it
		EnemyProjectile = 1 << 4,
		AlliedProjectile = 1 << 5,
		AirSceneLayer = 1 << 6,
		Pickup = 1 << 7,

		Aircraft = PlayerAircraft | AlliedAircraft | EnemyAircraft,//used to include all ORed objects into one definition
		Projectile = EnemyProjectile | AlliedProjectile,
	};
}