/*
 * DecisionTreeFactor.h
 *
 *  @date Feb 14, 2011
 *  @author Duy-Nguyen Ta
 */

#pragma once

#include <gtsam/discrete/DiscreteFactor.h>
#include <gtsam/discrete/Potentials.h>

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>
#include <exception>
#include <stdexcept>

namespace gtsam {

	class DiscreteConditional;

	/**
	 * A discrete probabilistic factor
	 */
	class DecisionTreeFactor: public DiscreteFactor, public Potentials {

	public:

		// typedefs needed to play nice with gtsam
		typedef DecisionTreeFactor This;
		typedef DiscreteConditional ConditionalType;
		typedef boost::shared_ptr<DecisionTreeFactor> shared_ptr;

	public:

		/// @name Standard Constructors
		/// @{

		/** Default constructor for I/O */
		DecisionTreeFactor();

		/** Constructor from Indices, Ordering, and AlgebraicDecisionDiagram */
		DecisionTreeFactor(const DiscreteKeys& keys, const ADT& potentials);

		/** Constructor from Indices and (string or doubles) */
		template<class SOURCE>
		DecisionTreeFactor(const DiscreteKeys& keys, SOURCE table) :
				DiscreteFactor(keys.indices()), Potentials(keys, table) {
		}

		/** Construct from a DiscreteConditional type */
		DecisionTreeFactor(const DiscreteConditional& c);

		/// @}
		/// @name Testable
		/// @{

		/// equality
		bool equals(const DecisionTreeFactor& other, double tol = 1e-9) const;

		// print
		void print(const std::string& s = "DecisionTreeFactor: ") const;

		/// @}
		/// @name Standard Interface
		/// @{

		/// Value is just look up in AlgebraicDecisonTree
		virtual double operator()(const Values& values) const {
			return Potentials::operator()(values);
		}

		/// multiply two factors
		DecisionTreeFactor operator*(const DecisionTreeFactor& f) const {
			return apply(f, ADT::Ring::mul);
		}

		/// divide by factor f (safely)
		DecisionTreeFactor operator/(const DecisionTreeFactor& f) const {
			return apply(f, safe_div);
		}

		/// Convert into a decisiontree
		virtual operator DecisionTreeFactor() const {
			return *this;
		}

		/// Create new factor by summing all values with the same separator values
		shared_ptr sum(size_t nrFrontals) const {
			return combine(nrFrontals, ADT::Ring::add);
		}

		/// Create new factor by maximizing over all values with the same separator values
		shared_ptr max(size_t nrFrontals) const {
			return combine(nrFrontals, ADT::Ring::max);
		}

		/// @}
		/// @name Advanced Interface
		/// @{

		/**
		 * Apply binary operator (*this) "op" f
		 * @param f the second argument for op
		 * @param op a binary operator that operates on AlgebraicDecisionDiagram potentials
		 */
		DecisionTreeFactor apply(const DecisionTreeFactor& f, ADT::Binary op) const;

		/**
		 * Combine frontal variables using binary operator "op"
		 * @param nrFrontals nr. of frontal to combine variables in this factor
		 * @param op a binary operator that operates on AlgebraicDecisionDiagram potentials
		 * @return shared pointer to newly created DecisionTreeFactor
		 */
		shared_ptr combine(size_t nrFrontals, ADT::Binary op) const;

		/*
		 * Ensure Arc-consistency
		 * @param j domain to be checked
		 * @param domains all other domains
		 */
		///
		bool ensureArcConsistency(size_t j, std::vector<Domain>& domains) const {
//			throw std::runtime_error(
//					"DecisionTreeFactor::ensureArcConsistency not implemented");
			return false;
		}

		/// Partially apply known values
		virtual DiscreteFactor::shared_ptr partiallyApply(const Values&) const {
			throw std::runtime_error("DecisionTreeFactor::partiallyApply not implemented");
		}

		/// Partially apply known values, domain version
		virtual DiscreteFactor::shared_ptr partiallyApply(
				const std::vector<Domain>&) const {
			throw std::runtime_error("DecisionTreeFactor::partiallyApply not implemented");
		}
		/// @}
	};
// DecisionTreeFactor

}// namespace gtsam