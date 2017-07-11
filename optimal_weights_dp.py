import logging
from input_data import target_weights, component_weights


logging.basicConfig(filename='optimal_weights.log', filemode='w',
                    level=logging.INFO, format='%(message)s')


def calculate_optimal_weights(target_weights, component_weights):
    optimal_set = {}
    max_weight = max(target_weights)
    min_weight = min(component_weights)
    weight_sets = {0: []}
    for target in range(min_weight, max_weight + 1):
        for component in component_weights:
            if component <= target and target - component in weight_sets:
                num_of_components = len(weight_sets[target - component]) + 1
                if (target not in weight_sets or
                    num_of_components < len(weight_sets[target])):
                    weight_sets[target] = (weight_sets[target - component] +
                                           [component])
        if target in target_weights:
            if target in weight_sets:
                target_set = {}
                for component in weight_sets[target]:
                    if component in target_set:
                        target_set[component] += 1
                    else:
                        target_set[component] = 1
                logging.info(
                    "target weight {}: {} ({})".format(
                        target, target_set, len(weight_sets[target])))
                for component, count in target_set.items():
                    if component in optimal_set:
                        optimal_set[component] = max(count, optimal_set[component])
                    else:
                        optimal_set[component] = count
            else:
                logging.info("target weight {}: []".format(target))
    logging.info("optimal weights set: {}".format(optimal_set))


if __name__ == "__main__":
    calculate_optimal_weights(target_weights, component_weights)
