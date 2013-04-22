#include "sampler.h"
#include "utils.h"
#include <assert.h>

namespace core
{
    ////////////////////////////////////////////////////
    ///         BASE SAMPLER IMPLEMENTATION 
    sampler::sampler(uint numSamples, uint numSets) : _numSamples(numSamples), _numSets(numSets),_jump(0), _count(0)
    {
        setup_shuffled_indices();
    }

    vector2 sampler::sample_unit_square()
    {
        /// Perform random jump if the pixel is sampled completely
        if (_count % _numSamples == 0)
        {
            _jump = (rand_uint() % _numSets) * _numSamples;
        }

        /// Start returning shuffled samples
        return _samples[_jump + _shuffledIndices[_jump + _count++ % _numSamples]];
    }

    vector2 sampler::sample_unit_disk()
    {
        assert(_diskSamples.size() != 0);

        /// Perform random jump if the pixel is sampled completely
        if (_count % _numSamples == 0)
        {
            _jump = (rand_uint() % _numSets) * _numSamples;
        }

        /// Start returning shuffled samples
        return _diskSamples[_jump + _shuffledIndices[_jump + _count++ % _numSamples]];
    }

    vector3 sampler::sample_unit_hemisphere()
    {
        assert(_hemisphereSamples.size() != 0);

        /// Perform random jump if the pixel is sampled completely
        if (_count % _numSamples == 0)
        {
            _jump = (rand_uint() % _numSets) * _numSamples;
        }

        /// Start returning shuffled samples
        return _hemisphereSamples[_jump + _shuffledIndices[_jump + _count++ % _numSamples]];
    }

    void sampler::map_samples_to_unit_disk()
    {
        uint numSamples = (uint)_samples.size();
        _diskSamples.reserve(numSamples);

        for (uint i = 0; i < numSamples;++i)
        {
            real phi, r;
            /// map samples to [-1,1]^2
            real a = 2 * _samples[i].x() - 1;
            real b = 2 * _samples[i].y() - 1;

            if (a > -b)
            {
                if (a > b)
                {
                    r = a;
                    phi = (real)(M_PI_4*(b/a));
                }
                else /// a <= b
                {
                    r = b;
                    phi = (real)(M_PI_4*(2 - a/b));
                }
            }
            else /// a <= -b
            {
                if (a < b)
                {
                    r = -a;
                    phi = (real)(M_PI_4*(4 + a/b)); 
                }
                else /// a >= b
                {
                    r = -b;
                    phi = (real)((b == 0)?(0):(M_PI_4 * (6 - a/b)));
                }
            }

            _diskSamples.push_back(vector2(r*cos(phi), r*sin(phi)));
        }
    }

    void sampler::map_samples_to_unit_hemisphere( const real& e )
    {
        uint numSamples = (uint)_samples.size();
        _hemisphereSamples.reserve(numSamples);

        for (uint i = 0; i < numSamples;++i)
        {
            real cos_phi  = (real)(cos((real)(2*M_PI*_samples[i].x())));
            real sin_phi  = (real)(sin((real)(2*M_PI*_samples[i].x())));
            real cos_theta = pow((real)1-_samples[i].y(), (real)1/(1 + e));
            real sin_theta = (real)1 - cos_theta*cos_theta;

            /// calculate spherical coordinates relative to u,v,w frame
            /// where w goes thru the center of a sphere and the sphere's top
            vector3 v(sin_theta*cos_phi, sin_theta*sin_phi, cos_theta);
            _hemisphereSamples.push_back(v);
        }
    }

    void sampler::setup_shuffled_indices()
    {
        /// indices array
        std::vector<uint> indices(_numSamples);
        /// shuffled indices
        _shuffledIndices.reserve(_numSets * _numSamples);

        /// initialize
        for (uint i = 0; i < _numSamples; ++i)
        {
            indices[i] = i;
        }

        /// fill up a shuffled indices
        for (uint i = 0; i < _numSets; ++i)
        {
            std::random_shuffle(indices.begin(), indices.end());

            for (uint j = 0; j < _numSamples; ++j)
                _shuffledIndices.push_back(indices[j]);
        }
    }

    ////////////////////////////////////////////////////
    ///         REGULAR SAMPLER IMPLEMENTATION 
    void regular_sampler::generate_samples()
    {
        uint n = (uint)sqrt((real)num_samples());
        uint numSets = num_sets();
        for(uint s = 0; s < numSets; ++s)
        {
            for(uint i = 0; i < n; ++i)
                for(uint j = 0; j < n; ++j)
                {
                    vector2 v((real)(i + 0.5)/n, (real)(j + 0.5)/n);
                    add_sample(v);
                }
        }
    }

    ////////////////////////////////////////////////////
    ///   RANDOM SAMPLER IMPLEMENTATION 
    void random_sampler::generate_samples()
    {
        uint n = num_samples();
        uint numSets = num_sets();
        for(uint s = 0; s < numSets; ++s)
        {
            for(uint i = 0; i < n; ++i)
            {
                vector2 v(rand_float(), rand_float());
                add_sample(v);
            }
        }
    }

    ////////////////////////////////////////////////////
    ///   JITTERED SAMPLER IMPLEMENTATION 
    void jittered_sampler::generate_samples()
    {
        uint n = (uint)sqrt((real)num_samples());
        uint numSets = num_sets();
        for(uint s = 0; s < numSets; ++s)
        {
            for(uint i = 0; i < n; ++i)
                for(uint j = 0; j < n; ++j)
                {
                    vector2 v((real)(i + rand_float()) /n, (real)(j + rand_float())/n);
                    add_sample(v);
                }
        }
    }

    ////////////////////////////////////////////////////
    ///   N-ROOKS SAMPLER IMPLEMENTATION 
    void nrooks_sampler::generate_samples()
    {
        uint n = num_samples();
        uint numSets = num_sets();
        for(uint s = 0; s < numSets; ++s)
        {
            for(uint i = 0; i < n; ++i)
            {
                vector2 v((real)(i + rand_float())/n, (real)(i + rand_float())/n);
                add_sample(v);
            }
        }

        shuffle();
    }

    void nrooks_sampler::shuffle()
    {
        uint n = num_samples();
        uint numSets = num_sets();
        for(uint i = 0; i  < numSets; ++i)
        {
            /// shuffle x coordinate
            for (uint j = 0; j < n - 1; ++j)
            {
                uint offset = rand_uint() % n;
                if (offset == j) continue;
                uint target = i * n + offset;
                uint source = i * n + j;

                real temp = get_sample(target).x();
                get_sample(target).x() = get_sample(source).x();
                get_sample(source).x() = temp;
            }

            /// shuffle y coordinate
            for (uint j = 0; j < n - 1; ++j)
            {
                uint offset = rand_uint() % n;
                if (offset == j) continue;
                uint target = i * n + offset;
                uint source = i * n + j;

                real temp = get_sample(target).y();
                get_sample(target).y() = get_sample(source).y();
                get_sample(source).y() = temp;
            }
        }
    }

    ////////////////////////////////////////////////////
    ///   HAMMERSLEY SAMPLER IMPLEMENTATION 
    void hammersley_sampler::generate_samples()
    {
        uint n = num_samples();
        uint numSets = num_sets();
        for(uint s = 0; s < numSets; ++s)
        {
            for(uint i = 0; i < n; ++i)
            {
                vector2 v((real)i/n, phi(i));
                add_sample(v);
            }
        }
    }

    real hammersley_sampler::phi( uint n )
    {
        uint temp = n;
        real base = 0.5;
        real res = 0;

        while(temp)
        {
            res += base * (temp & 1);
            temp >>= 1;
            base *= 0.5;
        }

        return res;
    }

    ////////////////////////////////////////////////////
    ///    MULTI-JITTERED SAMPLER IMPLEMENTATION 
    void multijittered_sampler::generate_samples()
    {
        // subgrid size
        uint subGridSize = num_samples();
        // grid size
        uint gridSize = (uint)sqrt((real)subGridSize);
        // sub-cells per cell
        uint subCellsPerCell = (uint)subGridSize/gridSize;
        // subgrid cell size
        real subCellSize = (real)1/subGridSize;
        // Num sets 
        uint numSets = num_sets();

        for (uint i = 0; i < numSets; ++i)
        {
            for (uint yj = 0; yj < gridSize; ++yj)
            {
                for (uint xj = 0; xj < gridSize; ++xj)
                {
                    vector2 v((xj*subCellsPerCell + yj + rand_float())*subCellSize, (yj * subCellsPerCell + xj + rand_float())*subCellSize);
                    add_sample(v);
                }
            }
        }

        shuffle();
    }
}