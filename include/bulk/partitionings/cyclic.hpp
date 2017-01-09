#include "partitioning.hpp"

namespace bulk {

/**
 * This cyclic partitioning is an initial draft of what will eventually part of
 * a bigger class of partitionings.
 *
 * In particular, block distributions are nearly identical, except for indexing
 * functions being slightly different, make common base class for this.
 *
 * Future challenges include how to coalesce lookups for distributed owner/index
 * tables.
 */
template <int D, int G = D>
class cyclic_partitioning : public multi_partitioning<D, G> {
   public:
    using multi_partitioning<D, G>::local_size;
    using multi_partitioning<D, G>::local_to_global;

    /**
     * Constructs a cyclic partitioning in nD.
     *
     * `grid`: the number of processors in each dimension
     * `data_size`: the global number of processors along each axis
     */
    cyclic_partitioning(bulk::world& world, index_type<D> data_size,
                        index_type<G> grid)
        : multi_partitioning<D, G>(world, data_size, grid) {
        static_assert(G <= D,
                      "Dimensionality of the data should be larger or equal to "
                      "that of the processor grid.");
    }

    /** Compute the local indices of a element using its global indices */
    index_type<D> global_to_local(index_type<D> index) override final {
        for (int d = 0; d < G; ++d) {
            index[d] = index[d] / this->grid_size_[d];
        }
        return index;
    }

    /** Local to global */
    index_type<D> local_to_global(index_type<D> xs,
                                  index_type<G> processor) override final {
        // FIXME implement
        return index_type<D>{};
    }

    /** The total number of elements along each axis on the processor index with
     * `idxs...` */
    index_type<D> local_size(index_type<G> idxs) override final {
        index_type<D> size;
        for (int dim = 0; dim < G; ++dim) {
            size[dim] = (this->global_size_[dim] + this->grid_size_[dim] -
                         idxs[dim] - 1) /
                        this->grid_size_[dim];
        }
        for (int dim = G; dim < D; ++dim) {
            size[dim] = this->global_size_[dim];
        }
        return size;
    }

    /** Cyclic in first 'G' dimensions. */
    index_type<G> grid_owner(index_type<D> xs) override final {
        index_type<G> result;
        for (int d = 0; d < G; ++d) {
            result[d] = xs[d] % this->grid_size_[d];
        }
        return result;
    }
};

}  // namespace bulk
