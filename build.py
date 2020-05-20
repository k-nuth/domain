import copy
import os
import cpuid
import platform
from kthbuild import get_base_march_ids, get_builder, handle_microarchs, copy_env_vars, filter_valid_exts, filter_marchs_tests

if __name__ == "__main__":
    full_build = os.getenv('KTH_FULL_BUILD', '0') == '1'
    builder, name = get_builder(os.path.dirname(os.path.abspath(__file__)))
    builder.add_common_builds(shared_option_name="%s:shared" % name)

    filtered_builds = []
    for settings, options, env_vars, build_requires, reference in builder.items:

        if settings["build_type"] == "Release" \
                and not options["%s:shared" % name]:

            copy_env_vars(env_vars)

            if os.getenv('KTH_RUN_TESTS', 'false') == 'true':
                options["%s:tests" % name] = "True"
                options["%s:examples" % name] = "False"


            march_ids = get_base_march_ids()

            ci_currency = os.getenv('KTH_CI_CURRENCY', None)
            # with_keoken = os.getenv('KTH_WITH_KEOKEN', 'false') == 'true'
            with_keoken = False

            if ci_currency is None:
                opts_bch = copy.deepcopy(options)
                # opts_btc = copy.deepcopy(options)
                # opts_ltc = copy.deepcopy(options)

                opts_bch["%s:currency" % name] = "BCH"
                opts_bch["%s:keoken" % name] = with_keoken

                # opts_btc["%s:currency" % name] = "BTC"
                # opts_ltc["%s:currency" % name] = "LTC"

                handle_microarchs("%s:march_id" % name, march_ids, filtered_builds, settings, opts_bch, env_vars, build_requires)
                # handle_microarchs("%s:march_id" % name, march_ids, filtered_builds, settings, opts_btc, env_vars, build_requires)
                # handle_microarchs("%s:march_id" % name, march_ids, filtered_builds, settings, opts_ltc, env_vars, build_requires)
            else:
                options["%s:currency" % name] = ci_currency
                options["%s:keoken" % name] = with_keoken
                handle_microarchs("%s:march_id" % name, march_ids, filtered_builds, settings, options, env_vars, build_requires)

            filter_marchs_tests(name, filtered_builds, ["%s:tests" % name, "%s:examples" % name])

    builder.builds = filtered_builds
    builder.run()
