Munge (MNG) Script Documentation
================================

Expressions
---------

+ *number*
+ Add ( *expression*, *expression* )
	`left + right`
+ Subtract ( *expression*, *expression* )
	`left - right`
+ Multiply ( *expession*, *expression* )
	`left * right`
+ Divide ( *expression*, *expression* )
	`left / right`
+ SineWave ( *expression*, *expression* )
	`sine( left / right * 2pi )`
+ CosineWave ( *expression*, *expression* )
	`cosine( left / right * 2pi )`
+ Random ( *expression*, *expression* )
	A random float between `left` and `right`
+ Variable ( *name* )
	Returns current value of variable `name`

Track
-----
Track ( *name* ) { *block...* }  
FadeIn ( *expression seconds* )  
FadeOut ( *expression seconds* )

The basic unit of music is a Track. Typically only one track plays at once; switching between tracks is accomplished by fading in and out, adhering to the FadeIn and FadeOut track parameters. Each Track has one or more Layers, which are played simultaneously. Tracks may also declare a BeatLength (in seconds), for use by BeatSynch in child layers. Volume may also be set in a Track (applied to Layer Volumes, I think?).

	Track(UpperTemple)
		{
		FadeIn(5)
		FadeOut(5)
		BeatLength(0.3)
		Volume(1.0)

		LoopLayer(Chord)
			{
			...
			}

		AleotoricLayer(StickMelody)
			{
			...
			}
		}

Layers
------

Layers are the "instruments" of a track, in that they either play one sample repeatedly (in the case of LoopLayers) or one or more samples, enclosed within Voices (AleotoricLayers).

### LoopLayers
LoopLayer ( *name* ) { *block...* }  
Wave ( *name* )  
Update { *block...* }  
Interval ( *seconds* )  
UpdateRate ( *seconds* )  
BeatSynch ( *seconds* )

A LoopLayer consists of a single Wave and an Update block. The Wave is played constantly and repeatedly. The Update is called at regular intervals and typically causes some change in the presentation of the samples (for example, it may pan the output from side to side, or alter the volume). Variables may be declared. If an Interval is specified, playing pauses for the length of the interval between each iteration of the layer. Either UpdateRate or BeatSynch (?) specify the how often the Update block is called. 

	LoopLayer(HighBreath)
		{
		Variable(counter,0.0)
		Variable(temp,0.0)
		Interval(0.1)
		Update
			{
			// Gradually, pan around at a random rate
			temp = Random(0.0, 0.1)
			counter = Add(counter, temp)
			Pan = CosineWave(counter, 30)
			// Scale the volume according to mood
			Volume = Multiply(Mood,0.4)
			Volume = Add(Volume,0.6)
			}
		UpdateRate(0.1)
		Wave(HighBreathG)
		}

### AleotoricLayers and Voices

An AleotoricLayer consists of one or more Voices to be played sequentially. Effects and Volume may be specified for the layer. The Interval of a layer specifies how long it is before the next Voice of an AleotoricLayer is to be played &mdash; it is possible to change this within the Voice.

Voices are individual Waves with optional Conditions and Intervals. Conditions are used to decide whether or not the Wave should be played &mdash; the value of the specified variable must be between the two specified values. Intervals allow the script to specify how long to wait before the next sample.

	AleotoricLayer(BendyEcho)
		{
		Volume(0.4)
		Effect(PingPong160)
		Interval(4)
		Voice
			{
			Condition(Mood,0.2,0.6)
			Wave(Bnd0)
			Interval ( Random( 4.0, 9.4) )
			}
		Voice
			{
			Condition(Mood,0.4,1.0)
			Wave(Bnd1)
			Interval ( Random( 4.0, 9.4) )
			}
		}


Comments
--------
Comments are indicated by a double slash (//) and may be placed within Track, Effect, Voice or Update declarations, as well as at the top level. (anywhere?)

TODO
----

+ AleotoricLayer
+ BeatLength
+ BeatSynch
+ Condition
+ Delay
+ Effect
+ Interval
+ LoopLayer
+ Pan
+ Stage
+ TempoDelay
+ Track
+ Update
+ UpdateRate
+ Variable
+ Voice
+ Volume
+ Wave
