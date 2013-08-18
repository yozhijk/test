/////////////////////////////////////////////////////
/// @file sampler.h
///	 Some sampler implementations
///
/// @author Dmitry Kolzov
///
#ifndef sampler_H
#define sampler_H

#include <vector>
#include <algorithm>

#include "common_types.h"

#ifdef _DEBUG
#include <iostream>
#endif

#define DEFAULT_SAMPLE_COUNT 1
#define DEFAULT_SETS_COUNT 83

namespace core
{
	/// class representing a sampler object
	/// is capable of sampling unit square
	/// unit disk and hemisphere
	/// to declare a particular sampler all
	/// you should do is to override
	/// generate_samples() method
	class sampler
	{
	public:
		/// constructor
		sampler(uint numSamples = DEFAULT_SAMPLE_COUNT, uint numSets = DEFAULT_SETS_COUNT);

		/// return a sample in the unit square
		/// sampling is performed num_samples()
		/// times from a single sampling pattern
		vector2 sample_unit_square();

		/// return a sample in the unit disk
		/// the samples are generated by
		/// concentrically mapping unit square
		/// to unit disk
		/// before calling this method
		/// make sure map_samples_to_unit_disk
		/// has been called
		vector2 sample_unit_disk();

		/// return a sample on a unit hemisphere
		/// samples are distributed with
		/// cosine power law (the power
		/// is specified in map_samples call
		vector3 sample_unit_hemisphere();

		/// map samples to unit disk 
		/// using concetric map by Shirley
		void map_samples_to_unit_disk();

		/// map samples to unit hemisphere
		/// using a cosine power law
		void map_samples_to_unit_hemisphere( const real& e );

		/// accessors
		void set_num_samples( uint numSamples ) { _numSamples = numSamples; }
		void set_num_sets   ( uint numSets )	{ _numSets = numSets; }
		uint num_samples	() const			{ return _numSamples; }
		uint num_sets	   () const			{ return _numSets; }

	protected:
		/// generate num_samples() sample patterns
		virtual void generate_samples(){}
		/// prepare shuffled indices
		void	setup_shuffled_indices();
		/// add sample to a samples array
		void	add_sample( const vector2& s ) { _samples.push_back(s); }
		/// access sample
		/// be careful, index checking is not performed
		vector2& get_sample( uint index ) { return _samples[index]; }

	private:
		/// a number of samples in a pattern
		uint _numSamples;
		/// a number of patterns
		uint _numSets;
		/// random pattern offset
		uint _jump;
		/// number of samples requested
		uint _count;
		/// samples on unit square
		std::vector<vector2> _samples;
		/// samples on a unit disk
		std::vector<vector2> _diskSamples;
		/// samples on a hemisphere
		std::vector<vector3> _hemisphereSamples;
		/// shuffled sample indices
		std::vector<uint> _shuffledIndices;
	};

	/// regular sampler
	class regular_sampler : public sampler
	{
	public:
		regular_sampler(uint numSamples = DEFAULT_SAMPLE_COUNT, uint numSets = DEFAULT_SETS_COUNT) : 
		  sampler(numSamples, numSets){generate_samples();}
	protected:
		virtual void generate_samples();
	};

	/// random sampler
	class random_sampler : public sampler
	{
	public:
		random_sampler(uint numSamples = DEFAULT_SAMPLE_COUNT, uint numSets = DEFAULT_SETS_COUNT) : 
		  sampler(numSamples, numSets){generate_samples();}
	protected:
		virtual void generate_samples();
	};

	/// jittered sampler
	class jittered_sampler : public sampler
	{
	public:
		jittered_sampler(uint numSamples = DEFAULT_SAMPLE_COUNT, uint numSets = DEFAULT_SETS_COUNT) : 
		  sampler(numSamples, numSets){generate_samples();}
	protected:
		virtual void generate_samples();
	};

	/// N-rooks sampler
	class nrooks_sampler : public sampler
	{
	public:
		nrooks_sampler(uint numSamples = DEFAULT_SAMPLE_COUNT, uint numSets = DEFAULT_SETS_COUNT) : 
		  sampler(numSamples, numSets){generate_samples();}

	protected:
		virtual void generate_samples();
		virtual void shuffle();
	};

	/// Hammerseley sampler
	class hammersley_sampler : public sampler
	{
	public:
		hammersley_sampler(uint numSamples = DEFAULT_SAMPLE_COUNT, uint numSets = DEFAULT_SETS_COUNT) : 
		  sampler(numSamples, numSets){generate_samples();}

	protected:
		virtual void generate_samples();
		real phi( uint n );
	};

	/// multijittered sampler
	class multijittered_sampler : public nrooks_sampler
	{
	public:
		multijittered_sampler(uint numSamples = DEFAULT_SAMPLE_COUNT, uint numSets = DEFAULT_SETS_COUNT) : 
		  nrooks_sampler(numSamples, numSets){generate_samples();}

	protected:
		virtual void generate_samples();
	};
}

#endif