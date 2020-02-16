#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "nle-lepton.h"
#include "util.h"
#include "phase2.h"

void verifyMatches(nle_config_t *nle_config, nle_state_t *nle_state) {
  //  separate the match table into a separate list for each exponent, then test all unique combinations of coefficients
  int i,j;
  nle_phase1_match_t *phase1_match;
  nle_phase1_match_t *term1_match;
  nle_phase1_match_t *term2_match;
  nle_phase1_match_t *term3_match;
  nle_phase1_match_t *temp_match;
  int t1,t2,t3;
  int dupe;
  struct timespec start_time;
  struct timespec end_time;
  double elapsed_time;
  double precision;
  int tmpmatchup;
  int tmpmatchdown;
  int tmpmatchcomplexity;
  long long tmphash;
  long combo_count;
  long combo;
  nle_input_use_t term1_uses;
  nle_input_use_t term2_uses;
  nle_input_use_t term3_uses;
  int complexity;
  int symmetry;

  // extract all term1 coefficients
  phase1_match=nle_state->phase1_matches_start;
  term1_match=nle_state->term1.matches_start;
  nle_state->term1.matches_count=0;
  for (i=0; i < nle_state->phase1_matches_count; i++) {
    if (phase1_match->exp_inv == nle_state->term1.exp_inv) {
     // determine integer/rational match value
      if (phase1_match->match > 1.0) {
       tmpmatchup=(int)(phase1_match->match + 0.5);
       tmpmatchdown=1;
      } else {
        tmpmatchup=1;
        tmpmatchdown=(int)((1.0 / phase1_match->match) + 0.5);
      }
      tmphash=(long long)phase1_match->smrfactor_mass ^ ((long long)((((tmpmatchup / tmpmatchdown) * (1.0 / phase1_match->static_multiplier)) * (long long)1.0E9) + 0.5));
      tmpmatchcomplexity=(phase1_match->match_complexity + (tmpmatchup * phase1_match->outfactor_rational_down) + (tmpmatchdown * phase1_match->outfactor_rational_up));
      // search existing match table for dupes and see if we have lower complexity
      temp_match=nle_state->term1.matches_start;
      dupe=0;
      for (j=0; j< nle_state->term1.matches_count; j++) {
        if (tmphash == temp_match->match_hash) {
          if (tmpmatchcomplexity < temp_match->match_complexity) {
            // replace
            temp_match->exp_inv=phase1_match->exp_inv;
            temp_match->smrfactor_mass=phase1_match->smrfactor_mass;
            temp_match->infactor_rational_up=phase1_match->infactor_rational_up;
            temp_match->infactor_rational_down=phase1_match->infactor_rational_down;
            temp_match->infactor_2_exp_up=phase1_match->infactor_2_exp_up;
            temp_match->infactor_2_exp_down=phase1_match->infactor_2_exp_down;
            temp_match->infactor_alpha_exp_up=phase1_match->infactor_alpha_exp_up;
            temp_match->infactor_alpha_exp_down=phase1_match->infactor_alpha_exp_down;
            temp_match->infactor_pi_exp_up=phase1_match->infactor_pi_exp_up;
            temp_match->infactor_pi_exp_down=phase1_match->infactor_pi_exp_down;
            temp_match->infactor_nss=phase1_match->infactor_nss;
            temp_match->infactor_nbv=phase1_match->infactor_nbv;
            temp_match->infactor_user_exp_up=phase1_match->infactor_user_exp_up;
            temp_match->infactor_user_exp_down=phase1_match->infactor_user_exp_down;
            temp_match->outfactor_rational_up=phase1_match->outfactor_rational_up;
            temp_match->outfactor_rational_down=phase1_match->outfactor_rational_down;
            temp_match->outfactor_2_exp_up=phase1_match->outfactor_2_exp_up;
            temp_match->outfactor_2_exp_down=phase1_match->outfactor_2_exp_down;
            temp_match->outfactor_alpha_exp_up=phase1_match->outfactor_alpha_exp_up;
            temp_match->outfactor_alpha_exp_down=phase1_match->outfactor_alpha_exp_down;
            temp_match->outfactor_pi_exp_up=phase1_match->outfactor_pi_exp_up;
            temp_match->outfactor_pi_exp_down=phase1_match->outfactor_pi_exp_down;
            temp_match->outfactor_sin2w_exp_up=phase1_match->outfactor_sin2w_exp_up;
            temp_match->outfactor_sin2w_exp_down=phase1_match->outfactor_sin2w_exp_down;
            temp_match->outfactor_cos2w_exp_up=phase1_match->outfactor_cos2w_exp_up;
            temp_match->outfactor_cos2w_exp_down=phase1_match->outfactor_cos2w_exp_down;
            temp_match->outfactor_user_exp_up=phase1_match->outfactor_user_exp_up;
            temp_match->outfactor_user_exp_down=phase1_match->outfactor_user_exp_down;
            temp_match->static_multiplier=phase1_match->static_multiplier;
            temp_match->match=phase1_match->match;
            temp_match->match_up=tmpmatchup;
            temp_match->match_down=tmpmatchdown;
            temp_match->match_complexity=tmpmatchcomplexity;
            temp_match->match_hash=tmphash;
            initUses(&temp_match->match_uses);
            addUses(&temp_match->match_uses, &phase1_match->match_uses);
          }
          dupe=1;
          break;
        }
        temp_match++;
      } // end for j
      if (dupe == 0) {
        // insert
        term1_match->exp_inv=phase1_match->exp_inv;
        term1_match->smrfactor_mass=phase1_match->smrfactor_mass;
        term1_match->infactor_rational_up=phase1_match->infactor_rational_up;
        term1_match->infactor_rational_down=phase1_match->infactor_rational_down;
        term1_match->infactor_2_exp_up=phase1_match->infactor_2_exp_up;
        term1_match->infactor_2_exp_down=phase1_match->infactor_2_exp_down;
        term1_match->infactor_alpha_exp_up=phase1_match->infactor_alpha_exp_up;
        term1_match->infactor_alpha_exp_down=phase1_match->infactor_alpha_exp_down;
        term1_match->infactor_pi_exp_up=phase1_match->infactor_pi_exp_up;
        term1_match->infactor_pi_exp_down=phase1_match->infactor_pi_exp_down;
        term1_match->infactor_nss=phase1_match->infactor_nss;
        term1_match->infactor_nbv=phase1_match->infactor_nbv;
        term1_match->infactor_user_exp_up=phase1_match->infactor_user_exp_up;
        term1_match->infactor_user_exp_down=phase1_match->infactor_user_exp_down;
        term1_match->outfactor_rational_up=phase1_match->outfactor_rational_up;
        term1_match->outfactor_rational_down=phase1_match->outfactor_rational_down;
        term1_match->outfactor_2_exp_up=phase1_match->outfactor_2_exp_up;
        term1_match->outfactor_2_exp_down=phase1_match->outfactor_2_exp_down;
        term1_match->outfactor_alpha_exp_up=phase1_match->outfactor_alpha_exp_up;
        term1_match->outfactor_alpha_exp_down=phase1_match->outfactor_alpha_exp_down;
        term1_match->outfactor_pi_exp_up=phase1_match->outfactor_pi_exp_up;
        term1_match->outfactor_pi_exp_down=phase1_match->outfactor_pi_exp_down;
        term1_match->outfactor_sin2w_exp_up=phase1_match->outfactor_sin2w_exp_up;
        term1_match->outfactor_sin2w_exp_down=phase1_match->outfactor_sin2w_exp_down;
        term1_match->outfactor_cos2w_exp_up=phase1_match->outfactor_cos2w_exp_up;
        term1_match->outfactor_cos2w_exp_down=phase1_match->outfactor_cos2w_exp_down;
        term1_match->outfactor_user_exp_up=phase1_match->outfactor_user_exp_up;
        term1_match->outfactor_user_exp_down=phase1_match->outfactor_user_exp_down;
        term1_match->static_multiplier=phase1_match->static_multiplier;
        term1_match->match=phase1_match->match;
        term1_match->match_up=tmpmatchup;
        term1_match->match_down=tmpmatchdown;
        term1_match->match_complexity=tmpmatchcomplexity;
        term1_match->match_hash=tmphash;
        initUses(&term1_match->match_uses);
        addUses(&term1_match->match_uses, &phase1_match->match_uses);
        nle_state->term1.matches_count++;
        term1_match++;
      } // end if not dupe
    }  // end if invexp
    phase1_match++;
  } // end for i

  // extract all term2 term coefficients
  phase1_match=nle_state->phase1_matches_start;
  term2_match=nle_state->term2.matches_start;
  nle_state->term2.matches_count=0;
  for (i=0; i < nle_state->phase1_matches_count; i++) {
    if (phase1_match->exp_inv == nle_state->term2.exp_inv) {
     // determine integer/rational match value
      if (phase1_match->match > 1.0) {
       tmpmatchup=(int)(phase1_match->match + 0.5);
       tmpmatchdown=1;
      } else {
        tmpmatchup=1;
        tmpmatchdown=(int)((1.0 / phase1_match->match) + 0.5);
      }
      tmphash=(long long)phase1_match->smrfactor_mass ^ ((long long)((((tmpmatchup / tmpmatchdown) * (1.0 / phase1_match->static_multiplier)) * (long long)1.0E9) + 0.5));
      tmpmatchcomplexity=(phase1_match->match_complexity + (tmpmatchup * phase1_match->outfactor_rational_down) + (tmpmatchdown * phase1_match->outfactor_rational_up));
      // search existing match table for dupes and see if we have lower complexity
      temp_match=nle_state->term2.matches_start;
      dupe=0;
      for (j=0; j< nle_state->term2.matches_count; j++) {
        if (tmphash == temp_match->match_hash) {
          if (tmpmatchcomplexity < temp_match->match_complexity) {
            // replace
            temp_match->exp_inv=phase1_match->exp_inv;
            temp_match->smrfactor_mass=phase1_match->smrfactor_mass;
            temp_match->infactor_rational_up=phase1_match->infactor_rational_up;
            temp_match->infactor_rational_down=phase1_match->infactor_rational_down;
            temp_match->infactor_2_exp_up=phase1_match->infactor_2_exp_up;
            temp_match->infactor_2_exp_down=phase1_match->infactor_2_exp_down;
            temp_match->infactor_alpha_exp_up=phase1_match->infactor_alpha_exp_up;
            temp_match->infactor_alpha_exp_down=phase1_match->infactor_alpha_exp_down;
            temp_match->infactor_pi_exp_up=phase1_match->infactor_pi_exp_up;
            temp_match->infactor_pi_exp_down=phase1_match->infactor_pi_exp_down;
            temp_match->infactor_nss=phase1_match->infactor_nss;
            temp_match->infactor_nbv=phase1_match->infactor_nbv;
            temp_match->infactor_user_exp_up=phase1_match->infactor_user_exp_up;
            temp_match->infactor_user_exp_down=phase1_match->infactor_user_exp_down;
            temp_match->outfactor_rational_up=phase1_match->outfactor_rational_up;
            temp_match->outfactor_rational_down=phase1_match->outfactor_rational_down;
            temp_match->outfactor_2_exp_up=phase1_match->outfactor_2_exp_up;
            temp_match->outfactor_2_exp_down=phase1_match->outfactor_2_exp_down;
            temp_match->outfactor_alpha_exp_up=phase1_match->outfactor_alpha_exp_up;
            temp_match->outfactor_alpha_exp_down=phase1_match->outfactor_alpha_exp_down;
            temp_match->outfactor_pi_exp_up=phase1_match->outfactor_pi_exp_up;
            temp_match->outfactor_pi_exp_down=phase1_match->outfactor_pi_exp_down;
            temp_match->outfactor_sin2w_exp_up=phase1_match->outfactor_sin2w_exp_up;
            temp_match->outfactor_sin2w_exp_down=phase1_match->outfactor_sin2w_exp_down;
            temp_match->outfactor_cos2w_exp_up=phase1_match->outfactor_cos2w_exp_up;
            temp_match->outfactor_cos2w_exp_down=phase1_match->outfactor_cos2w_exp_down;
            temp_match->outfactor_user_exp_up=phase1_match->outfactor_user_exp_up;
            temp_match->outfactor_user_exp_down=phase1_match->outfactor_user_exp_down;
            temp_match->static_multiplier=phase1_match->static_multiplier;
            temp_match->match=phase1_match->match;
            temp_match->match_up=tmpmatchup;
            temp_match->match_down=tmpmatchdown;
            temp_match->match_complexity=tmpmatchcomplexity;
            temp_match->match_hash=tmphash;
            initUses(&temp_match->match_uses);
            addUses(&temp_match->match_uses, &phase1_match->match_uses);
          }
          dupe=1;
          break;
        }
        temp_match++;
      } // end for j
      if (dupe ==0) {
        // insert
        term2_match->exp_inv=phase1_match->exp_inv;
        term2_match->smrfactor_mass=phase1_match->smrfactor_mass;
        term2_match->infactor_rational_up=phase1_match->infactor_rational_up;
        term2_match->infactor_rational_down=phase1_match->infactor_rational_down;
        term2_match->infactor_2_exp_up=phase1_match->infactor_2_exp_up;
        term2_match->infactor_2_exp_down=phase1_match->infactor_2_exp_down;
        term2_match->infactor_alpha_exp_up=phase1_match->infactor_alpha_exp_up;
        term2_match->infactor_alpha_exp_down=phase1_match->infactor_alpha_exp_down;
        term2_match->infactor_pi_exp_up=phase1_match->infactor_pi_exp_up;
        term2_match->infactor_pi_exp_down=phase1_match->infactor_pi_exp_down;
        term2_match->infactor_nss=phase1_match->infactor_nss;
        term2_match->infactor_nbv=phase1_match->infactor_nbv;
        term2_match->infactor_user_exp_up=phase1_match->infactor_user_exp_up;
        term2_match->infactor_user_exp_down=phase1_match->infactor_user_exp_down;
        term2_match->outfactor_rational_up=phase1_match->outfactor_rational_up;
        term2_match->outfactor_rational_down=phase1_match->outfactor_rational_down;
        term2_match->outfactor_2_exp_up=phase1_match->outfactor_2_exp_up;
        term2_match->outfactor_2_exp_down=phase1_match->outfactor_2_exp_down;
        term2_match->outfactor_alpha_exp_up=phase1_match->outfactor_alpha_exp_up;
        term2_match->outfactor_alpha_exp_down=phase1_match->outfactor_alpha_exp_down;
        term2_match->outfactor_pi_exp_up=phase1_match->outfactor_pi_exp_up;
        term2_match->outfactor_pi_exp_down=phase1_match->outfactor_pi_exp_down;
        term2_match->outfactor_sin2w_exp_up=phase1_match->outfactor_sin2w_exp_up;
        term2_match->outfactor_sin2w_exp_down=phase1_match->outfactor_sin2w_exp_down;
        term2_match->outfactor_cos2w_exp_up=phase1_match->outfactor_cos2w_exp_up;
        term2_match->outfactor_cos2w_exp_down=phase1_match->outfactor_cos2w_exp_down;
        term2_match->outfactor_user_exp_up=phase1_match->outfactor_user_exp_up;
        term2_match->outfactor_user_exp_down=phase1_match->outfactor_user_exp_down;
        term2_match->static_multiplier=phase1_match->static_multiplier;
        term2_match->match=phase1_match->match;
        term2_match->match_up=tmpmatchup;
        term2_match->match_down=tmpmatchdown;
        term2_match->match_complexity=tmpmatchcomplexity;
        term2_match->match_hash=tmphash;
        initUses(&term2_match->match_uses);
        addUses(&term2_match->match_uses, &phase1_match->match_uses);
        nle_state->term2.matches_count++;
        term2_match++;
      } // end if not dupe
    }  // end if invexp
    phase1_match++;
  } // end for i

  // extract all term3 coefficients
  phase1_match=nle_state->phase1_matches_start;
  term3_match=nle_state->term3.matches_start;
  nle_state->term3.matches_count=0;
  for (i=0; i < nle_state->phase1_matches_count; i++) {
    if (phase1_match->exp_inv == nle_state->term3.exp_inv) {
     // determine integer/rational match value
      if (phase1_match->match > 1.0) {
       tmpmatchup=(int)(phase1_match->match + 0.5);
       tmpmatchdown=1;
      } else {
        tmpmatchup=1;
        tmpmatchdown=(int)((1.0 / phase1_match->match) + 0.5);
      }
      tmphash=(long long)phase1_match->smrfactor_mass ^ ((long long)((((tmpmatchup / tmpmatchdown) * (1.0 / phase1_match->static_multiplier)) * (long long)1.0E9) + 0.5));
      tmpmatchcomplexity=(phase1_match->match_complexity + (tmpmatchup * phase1_match->outfactor_rational_down) + (tmpmatchdown * phase1_match->outfactor_rational_up));
      // search existing match table for dupes and see if we have lower complexity
      temp_match=nle_state->term3.matches_start;
      dupe=0;
      for (j=0; j< nle_state->term3.matches_count; j++) {
        if (tmphash == temp_match->match_hash) {
          if (tmpmatchcomplexity < temp_match->match_complexity) {
            // replace
            temp_match->exp_inv=phase1_match->exp_inv;
            temp_match->smrfactor_mass=phase1_match->smrfactor_mass;
            temp_match->infactor_rational_up=phase1_match->infactor_rational_up;
            temp_match->infactor_rational_down=phase1_match->infactor_rational_down;
            temp_match->infactor_2_exp_up=phase1_match->infactor_2_exp_up;
            temp_match->infactor_2_exp_down=phase1_match->infactor_2_exp_down;
            temp_match->infactor_alpha_exp_up=phase1_match->infactor_alpha_exp_up;
            temp_match->infactor_alpha_exp_down=phase1_match->infactor_alpha_exp_down;
            temp_match->infactor_pi_exp_up=phase1_match->infactor_pi_exp_up;
            temp_match->infactor_pi_exp_down=phase1_match->infactor_pi_exp_down;
            temp_match->infactor_nss=phase1_match->infactor_nss;
            temp_match->infactor_nbv=phase1_match->infactor_nbv;
            temp_match->infactor_user_exp_up=phase1_match->infactor_user_exp_up;
            temp_match->infactor_user_exp_down=phase1_match->infactor_user_exp_down;
            temp_match->outfactor_rational_up=phase1_match->outfactor_rational_up;
            temp_match->outfactor_rational_down=phase1_match->outfactor_rational_down;
            temp_match->outfactor_2_exp_up=phase1_match->outfactor_2_exp_up;
            temp_match->outfactor_2_exp_down=phase1_match->outfactor_2_exp_down;
            temp_match->outfactor_alpha_exp_up=phase1_match->outfactor_alpha_exp_up;
            temp_match->outfactor_alpha_exp_down=phase1_match->outfactor_alpha_exp_down;
            temp_match->outfactor_pi_exp_up=phase1_match->outfactor_pi_exp_up;
            temp_match->outfactor_pi_exp_down=phase1_match->outfactor_pi_exp_down;
            temp_match->outfactor_sin2w_exp_up=phase1_match->outfactor_sin2w_exp_up;
            temp_match->outfactor_sin2w_exp_down=phase1_match->outfactor_sin2w_exp_down;
            temp_match->outfactor_cos2w_exp_up=phase1_match->outfactor_cos2w_exp_up;
            temp_match->outfactor_cos2w_exp_down=phase1_match->outfactor_cos2w_exp_down;
            temp_match->outfactor_user_exp_up=phase1_match->outfactor_user_exp_up;
            temp_match->outfactor_user_exp_down=phase1_match->outfactor_user_exp_down;
            temp_match->static_multiplier=phase1_match->static_multiplier;
            temp_match->match=phase1_match->match;
            temp_match->match_up=tmpmatchup;
            temp_match->match_down=tmpmatchdown;
            temp_match->match_complexity=tmpmatchcomplexity;
            temp_match->match_hash=tmphash;
            initUses(&temp_match->match_uses);
            addUses(&temp_match->match_uses, &phase1_match->match_uses);
          }
          dupe=1;
          break;
        }
        temp_match++;
      } // end for j
      if (dupe ==0) {
        // insert
        term3_match->exp_inv=phase1_match->exp_inv;
        term3_match->smrfactor_mass=phase1_match->smrfactor_mass;
        term3_match->infactor_rational_up=phase1_match->infactor_rational_up;
        term3_match->infactor_rational_down=phase1_match->infactor_rational_down;
        term3_match->infactor_2_exp_up=phase1_match->infactor_2_exp_up;
        term3_match->infactor_2_exp_down=phase1_match->infactor_2_exp_down;
        term3_match->infactor_alpha_exp_up=phase1_match->infactor_alpha_exp_up;
        term3_match->infactor_alpha_exp_down=phase1_match->infactor_alpha_exp_down;
        term3_match->infactor_pi_exp_up=phase1_match->infactor_pi_exp_up;
        term3_match->infactor_pi_exp_down=phase1_match->infactor_pi_exp_down;
        term3_match->infactor_nss=phase1_match->infactor_nss;
        term3_match->infactor_nbv=phase1_match->infactor_nbv;
        term3_match->infactor_user_exp_up=phase1_match->infactor_user_exp_up;
        term3_match->infactor_user_exp_down=phase1_match->infactor_user_exp_down;
        term3_match->outfactor_rational_up=phase1_match->outfactor_rational_up;
        term3_match->outfactor_rational_down=phase1_match->outfactor_rational_down;
        term3_match->outfactor_2_exp_up=phase1_match->outfactor_2_exp_up;
        term3_match->outfactor_2_exp_down=phase1_match->outfactor_2_exp_down;
        term3_match->outfactor_alpha_exp_up=phase1_match->outfactor_alpha_exp_up;
        term3_match->outfactor_alpha_exp_down=phase1_match->outfactor_alpha_exp_down;
        term3_match->outfactor_pi_exp_up=phase1_match->outfactor_pi_exp_up;
        term3_match->outfactor_pi_exp_down=phase1_match->outfactor_pi_exp_down;
        term3_match->outfactor_sin2w_exp_up=phase1_match->outfactor_sin2w_exp_up;
        term3_match->outfactor_sin2w_exp_down=phase1_match->outfactor_sin2w_exp_down;
        term3_match->outfactor_cos2w_exp_up=phase1_match->outfactor_cos2w_exp_up;
        term3_match->outfactor_cos2w_exp_down=phase1_match->outfactor_cos2w_exp_down;
        term3_match->outfactor_user_exp_up=phase1_match->outfactor_user_exp_up;
        term3_match->outfactor_user_exp_down=phase1_match->outfactor_user_exp_down;
        term3_match->static_multiplier=phase1_match->static_multiplier;
        term3_match->match=phase1_match->match;
        term3_match->match_up=tmpmatchup;
        term3_match->match_down=tmpmatchdown;
        term3_match->match_complexity=tmpmatchcomplexity;
        term3_match->match_hash=tmphash;
        initUses(&term3_match->match_uses);
        addUses(&term3_match->match_uses, &phase1_match->match_uses);
        nle_state->term3.matches_count++;
        term3_match++;
      } // end if not dupe
    }  // end if invexp
    phase1_match++;
  } // end for i

  // generate all combinations and send to phase2
  term1_match=nle_state->term1.matches_start;
  combo=0;
  combo_count=(nle_state->term1.matches_count * nle_state->term2.matches_count * nle_state->term3.matches_count);
  if (nle_config->status_enable == 1) {
    printf("status, Solving phase 2 formulas for masses, random input: %d, exponents: %s,                 progress: total (0/%ld) term1 (0/%d) term2 (0/%d) term3 (0/%d)\n", nle_state->phase1_seq, nle_state->exponents_str, combo_count, nle_state->term1.matches_count, nle_state->term2.matches_count, nle_state->term3.matches_count);
    fflush(stdout);
  }
  for (t1=0; t1 < nle_state->term1.matches_count; t1++) {
    initUses(&term1_uses);
    addUses(&term1_uses, &term1_match->match_uses);
    term2_match=nle_state->term2.matches_start;
    for (t2=0; t2 < nle_state->term2.matches_count; t2++) {
      initUses(&term2_uses);
      addUses(&term2_uses, &term2_match->match_uses);
      term3_match=nle_state->term3.matches_start;
      for (t3=0; t3 < nle_state->term3.matches_count; t3++) {
        combo++;

        // calculate complexity score
        complexity=term1_match->match_complexity + term2_match->match_complexity + term3_match->match_complexity;

        // calculate symmetry score.   This measures how many factors are identical or inverse identical between the dfferent terms
        symmetry=0;
        checkSymmetry(&symmetry, (term1_match->match_up * term1_match->outfactor_rational_down), (term2_match->match_up * term2_match->outfactor_rational_down), (term3_match->match_up * term3_match->outfactor_rational_down));
        checkSymmetry(&symmetry, (term1_match->match_down * term1_match->outfactor_rational_up), (term2_match->match_down * term2_match->outfactor_rational_up), (term3_match->match_down * term3_match->outfactor_rational_up));
        checkSymmetry(&symmetry, term1_match->outfactor_2_exp_up, term2_match->outfactor_2_exp_up, term3_match->outfactor_2_exp_up);
        checkSymmetry(&symmetry, term1_match->outfactor_pi_exp_up, term2_match->outfactor_pi_exp_up, term3_match->outfactor_pi_exp_up);
        checkSymmetry(&symmetry, (term1_match->outfactor_alpha_exp_up * term1_match->outfactor_alpha_exp_down), (term2_match->outfactor_alpha_exp_up * term2_match->outfactor_alpha_exp_down), (term3_match->outfactor_alpha_exp_up * term3_match->outfactor_alpha_exp_down));
        checkSymmetry(&symmetry, (term1_match->outfactor_sin2w_exp_up * term1_match->outfactor_sin2w_exp_down), (term2_match->outfactor_sin2w_exp_up * term2_match->outfactor_sin2w_exp_down), (term3_match->outfactor_sin2w_exp_up * term3_match->outfactor_sin2w_exp_down));
        checkSymmetry(&symmetry, (term1_match->outfactor_cos2w_exp_up * term1_match->outfactor_cos2w_exp_down), (term2_match->outfactor_cos2w_exp_up * term2_match->outfactor_cos2w_exp_down), (term3_match->outfactor_cos2w_exp_up * term3_match->outfactor_cos2w_exp_down));
        checkSymmetry(&symmetry, term1_match->infactor_rational_up, term2_match->infactor_rational_up, term3_match->infactor_rational_up);
        checkSymmetry(&symmetry, term1_match->infactor_rational_down, term2_match->infactor_rational_down, term3_match->infactor_rational_down);
        checkSymmetry(&symmetry, term1_match->infactor_nbv, term2_match->infactor_nbv, term3_match->infactor_nbv);
        checkSymmetry(&symmetry, term1_match->infactor_nss, term2_match->infactor_nss, term3_match->infactor_nss);
        checkSymmetry(&symmetry, term1_match->infactor_2_exp_up, term2_match->infactor_2_exp_up, term3_match->infactor_2_exp_up);
        checkSymmetry(&symmetry, term1_match->infactor_pi_exp_up, term2_match->infactor_pi_exp_up, term3_match->infactor_pi_exp_up);
        checkSymmetry(&symmetry, (term1_match->infactor_alpha_exp_up * term1_match->infactor_alpha_exp_down), (term2_match->infactor_alpha_exp_up * term2_match->infactor_alpha_exp_down), (term3_match->infactor_alpha_exp_up * term3_match->infactor_alpha_exp_down));
        if ((symmetry >= nle_config->phase2_symmetry_min) && (complexity <= nle_config->phase2_complexity_max)) {
         if ((nle_config->phase2_check_nbv_nss == 0) || ((term1_match->infactor_nbv == term2_match->infactor_nbv) && (term1_match->infactor_nbv == term3_match->infactor_nbv) && (term1_match->infactor_nss == term2_match->infactor_nss) && (term1_match->infactor_nss == term3_match-> infactor_nss))) { // consistency check
           if ((nle_config->phase2_check_weak == 0) || ((term1_match->outfactor_sin2w_exp_up == term2_match->outfactor_sin2w_exp_up) && (term1_match->outfactor_sin2w_exp_up == term3_match->outfactor_sin2w_exp_up)\
                                                     && (term1_match->outfactor_sin2w_exp_down == term2_match->outfactor_sin2w_exp_down) && (term1_match->outfactor_sin2w_exp_down == term3_match->outfactor_sin2w_exp_down)\
                                                     && (term1_match->outfactor_cos2w_exp_up == term2_match->outfactor_cos2w_exp_up) && (term1_match->outfactor_cos2w_exp_up == term3_match->outfactor_cos2w_exp_up)\
                                                     && (term1_match->outfactor_cos2w_exp_down == term2_match->outfactor_cos2w_exp_down) && (term1_match->outfactor_cos2w_exp_down == term3_match->outfactor_cos2w_exp_down))) {

              initUses(&term3_uses);
              addUses(&term3_uses, &term3_match->match_uses);
              initUses(&nle_state->all_uses);
              addUses(&nle_state->all_uses, &term1_uses);
              addUses(&nle_state->all_uses, &term2_uses);
              addUses(&nle_state->all_uses, &term3_uses);
              nle_state->term1.current_match=term1_match;
              nle_state->term2.current_match=term2_match;
              nle_state->term3.current_match=term3_match;
              clock_gettime(CLOCK_REALTIME, &start_time);

              // send to phase2 to verify formula
              precision=solveNLEforMasses(nle_config, nle_state);

              if (nle_config->status_enable ==1) {
                clock_gettime(CLOCK_REALTIME, &end_time);
                elapsed_time=((double)(end_time.tv_sec - 1500000000) + ((double)end_time.tv_nsec / 1.0E9)) - ((double)(start_time.tv_sec - 1500000000) + ((double)start_time.tv_nsec) / 1.0E9);
                if (precision < 1.0E30) {
                  printf("status, Solved  phase 2 formula  for masses, random input: %d, exponents: %s, mass mode: %d%d%d, progress: total (%ld/%ld) term1 (%d/%d) term2 (%d/%d) term3 (%d/%d), precision: %.3e, (%6.4fs)\n", nle_state->phase1_seq, nle_state->exponents_str, term1_match->smrfactor_mass, term2_match->smrfactor_mass, term3_match->smrfactor_mass, combo, combo_count, t1+1, nle_state->term1.matches_count, t2+1, nle_state->term2.matches_count, t3+1, nle_state->term3.matches_count, precision, elapsed_time);
                  fflush(stdout);
                } else {
                  printf("status, Failed to solve phase 2 formula  for masses, random input: %d, exponents: %s, mass mode: %d%d%d, progress: total (%ld/%ld) term1 (%d/%d) term2 (%d/%d) term3 (%d/%d), precision: %.3e, (%6.4fs)\n", nle_state->phase1_seq, nle_state->exponents_str, term1_match->smrfactor_mass, term2_match->smrfactor_mass, term3_match->smrfactor_mass, combo, combo_count, t1+1, nle_state->term1.matches_count, t2+1, nle_state->term2.matches_count, t3+1, nle_state->term3.matches_count, precision, elapsed_time);
                  fflush(stdout);
                } // end precision
              } // end status_enable
            } // end weak consistency check
          } // end nbv/nss consistency check
        } // end if symmetry and complexity check
        term3_match++;
      } // for t3
      term2_match++;
    } // for t2
    term1_match++;
  } // for t1
}